import { Inject, Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from '../../environments/environment';
import { finalize, map, take } from 'rxjs/operators';
import { AppStateService } from '../state/app-state.service';
import { Keys } from '../utils';
import { StorageService, LOCAL_STORAGE } from 'ngx-webstorage-service';
import { Router } from '@angular/router';
import { Observable } from 'rxjs';
import { ToplistService } from './toplist.service';
import { MessageService } from './message.service';
import { TranslateService } from '@ngx-translate/core';
import { GoldGiftDto } from '../dto/rest';
import {
  LocalAccountStatus,
  LocalLoginDto,
  NewLocalUserDto,
  UserDto
} from '../dto';
import { SoundService } from '.';
import { ChatService } from './chat.service';

@Injectable({
  providedIn: 'root'
})
export class AccountService {
  url: string;
  constructor(
    private http: HttpClient,
    @Inject(LOCAL_STORAGE) private storage: StorageService,
    private router: Router,
    private topListService: ToplistService,
    private messageService: MessageService,
    private trans: TranslateService,
    private sound: SoundService,
    private appState: AppStateService,
    private chatService: ChatService
  ) {
    this.url = `${environment.apiServiceUrl}/account`;
  }

  public async signIn(idToken: string, provider: string): Promise<void> {
    const options = {
      headers: { Authorization: idToken }
    };
    // Gets or creates the user in backgammon database.

    // Google user data is fetched by the backend.
    let userDto = { socialProvider: provider };

    if (provider === 'FACEBOOK') {
      // Getting some info from the provider. Only name email and photo.
      userDto = await this.getFacebookUser();
    }

    this.appState.showBusy();
    this.http
      .post<UserDto>(`${this.url}/signin`, userDto, options)
      .pipe(
        map((data) => {
          return data;
        }),
        take(1) // unsubscribes
      )
      .subscribe((userDto: UserDto) => {
        this.trans.use(userDto?.preferredLanguage ?? 'en');
        this.storage.set(Keys.loginKey, userDto);
        this.appState.user.setValue(userDto);
        if (userDto) this.appState.changeTheme(userDto?.theme);
        this.appState.hideBusy();
        if (userDto?.createdNew) {
          this.router.navigateByUrl('/edit-user');
        }
        if (userDto) {
          this.topListService.loadToplist();
          this.messageService.loadMessages();
        }
      });
  }

  async getFacebookUser(): Promise<UserDto> {
    const FB = (window as any).FB;
    let userDto: UserDto;
    const promise = new Promise<UserDto>((reslove) => {
      FB.api('/me?fields=id,name,email,picture', (response: any) => {
        userDto = { ...response };
        userDto.photoUrl = response?.picture?.data?.url;
        userDto.socialProvider = 'FACEBOOK';
        userDto.socialProviderId = response.id;
        return reslove(userDto);
      });
    });
    return await promise;
  }

  signOut(): void {
    this.appState.user.clearValue();
    this.storage.remove(Keys.loginKey);
    this.trans.use('en');
  }

  // If the user account is stored in local storage, it will be restored without contacting social provider
  repair(): void {
    const user = this.storage.get(Keys.loginKey) as UserDto;
    this.appState.user.setValue(user);
    this.trans.use(user?.preferredLanguage ?? 'en');
    if (user) {
      this.appState.changeTheme(user.theme);
      this.synchUser();
    }
  }

  saveUser(user: UserDto): Observable<void> {
    return this.http.post(`${this.url}/saveuser`, user).pipe(
      map(() => {
        this.appState.user.setValue(user);
        this.appState.theme.setValue(user.theme);
        this.storage.set(Keys.loginKey, user);
      })
    );
  }

  deleteUser(): void {
    const user = this.appState.user.getValue();
    this.http.post(`${this.url}/delete`, user).subscribe(() => {
      this.appState.user.clearValue();
      // this.authService.signOut();
      this.storage.set(Keys.loginKey, null);
      this.router.navigateByUrl('/lobby');
    });
  }

  isLoggedIn(): boolean {
    return !!this.appState.user.getValue();
  }

  getGold(): void {
    this.http
      .get(`${this.url}/requestgold`)
      .pipe(
        map((response) => {
          const dto = response as GoldGiftDto;
          const user = this.appState.user.getValue();
          if (dto.gold > user.gold) this.sound.playCoin();
          this.appState.user.setValue({
            ...user,
            gold: dto.gold,
            lastFreeGold: dto.lastFreeGold
          });
        }),
        take(1)
      )
      .subscribe();
  }

  synchUser(): void {
    const user = this.appState.user.getValue();
    this.appState.showBusyNoOverlay();
    this.http
      .get(`${this.url}/getuser?userId=${user.id}`)
      .pipe(
        map((response) => {
          const userDto = response as UserDto;
          this.trans.use(userDto.preferredLanguage ?? 'en');
          this.storage.set(Keys.loginKey, userDto);
          this.appState.user.setValue(userDto);
          if (userDto) this.appState.changeTheme(userDto.theme);
          this.appState.hideBusy();
        }),
        take(1)
      )
      .subscribe();
  }

  newLocalUser(dto: NewLocalUserDto): Observable<LocalAccountStatus> {
    this.appState.showBusy();
    return this.http.post(`${this.url}/newlocal`, dto).pipe(
      map((response) => {
        var userDto = response as UserDto;
        this.storage.set(Keys.loginKey, userDto);
        this.appState.user.setValue(userDto);
        if (userDto) return LocalAccountStatus.success;
        return LocalAccountStatus.nameExists;
      }),
      finalize(() => {
        this.appState.hideBusy();
      }),
      take(1)
    );
  }

  localLogin(dto: LocalLoginDto): Observable<LocalAccountStatus> {
    this.appState.showBusy();
    return this.http.post(`${this.url}/signinlocal`, dto).pipe(
      map((response) => {
        var userDto = response as UserDto;
        this.storage.set(Keys.loginKey, userDto);
        this.appState.user.setValue(userDto);
        if (userDto) {
          return LocalAccountStatus.success;
        }
        return LocalAccountStatus.invalidLogin;
      }),
      finalize(() => {
        this.appState.hideBusy();
      }),
      take(1)
    );
  }

  toggleIntro(): void {
    this.http
      .get(`${this.url}/toggleIntro`)
      .pipe(
        map((response) => {
          const mute = response as boolean;
          const user = this.appState.user.getValue();
          this.appState.user.setValue({ ...user, muteIntro: mute });
          if (mute) {
            this.sound.fadeIntro();
          } else {
            this.sound.unMuteIntro();
          }
        }),
        take(1)
      )
      .subscribe();
  }
}

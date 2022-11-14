import { Injectable, OnDestroy } from '@angular/core';
import { Router, UrlSerializer } from '@angular/router';
import { environment } from 'src/environments/environment';
import { AppStateService } from '../state/app-state.service';

@Injectable({
  providedIn: 'root'
})
export class ChatServiceService implements OnDestroy {
  socket: WebSocket | undefined;
  url: string = '';

  constructor(
    private appState: AppStateService,
    private router: Router,
    private serializer: UrlSerializer
  ) {}

  connect() {
    this.cleanup();

    this.url = environment.chatServiceUrl;
    if (environment.production) {
      this.url = origin.replace('https://', 'wss://') + '/chat';
    }

    const user = this.appState.user.getValue();
    const userId = user ? user.id : '';
    const tree = this.router.createUrlTree([], {
      queryParams: {
        userId: userId
      }
    });
    const url = this.url + this.serializer.serialize(tree);

    this.socket = new WebSocket(url);
    this.socket.onmessage = this.onMessage.bind(this);
    this.socket.onerror = this.onError.bind(this);
    this.socket.onopen = this.onOpen.bind(this);
    this.socket.onclose = this.onClose.bind(this);
  }

  onOpen(event: Event): void {
    // console.log('Open', { event });
    const now = new Date();
    // const ping = now.getTime() - this.connectTime.getTime();

    // this.appState.myConnection.setValue({ connected: true, pingMs: ping });
  }

  onError(event: Event): void {
    console.error('Error', { event });
  }

  onClose(event: CloseEvent): void {
    // console.log('Close', { event });
    // this.statusMessageService.setMyConnectionLost(event.reason);
  }

  // Messages received from server.
  onMessage(message: MessageEvent<string>): void {
    const action = JSON.parse(message.data); // as ActionDto;
  }

  ngOnDestroy(): void {
    this.cleanup();
  }

  private cleanup() {
    if (this.socket && this.socket.readyState !== this.socket.CLOSED) {
      this.socket.close();
    }
  }
}

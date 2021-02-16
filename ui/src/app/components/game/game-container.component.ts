import { Component, OnDestroy } from '@angular/core';
import { Observable, Subscription } from 'rxjs';
import { DiceDto, GameDto, MoveDto, PlayerColor } from 'src/app/dto';
import { SocketsService } from 'src/app/services';
import { AppState } from 'src/app/state/app-state';

@Component({
  selector: 'app-game',
  templateUrl: './game-container.component.html',
  styleUrls: ['./game-container.component.scss']
})
export class GameContainerComponent implements OnDestroy {
  constructor(private service: SocketsService) {
    this.gameDto$ = AppState.Singleton.game.changed;
    this.dices$ = AppState.Singleton.dices.changed;
    this.playerColor$ = AppState.Singleton.myColor.changed;
    this.gameSubs = AppState.Singleton.game.changed.subscribe(
      this.gameChanged.bind(this)
    );
    service.connect();
  }
  gameDto$: Observable<GameDto>;
  dices$: Observable<DiceDto[]>;
  playerColor$: Observable<PlayerColor>;
  gameSubs: Subscription;

  sendHidden = true;

  sendMoves(): void {
    this.sendHidden = true;
    this.service.sendMoves();
  }

  doMove(move: MoveDto): void {
    this.service.doMove(move);
    this.sendHidden = move.nextMoves && move.nextMoves.length > 0;
  }

  undoMove(): void {
    this.service.undoMove();
    this.sendHidden = true;
  }

  gameChanged(dto: GameDto): void {
    // todo: auto send here.
    this.sendHidden =
      (dto.validMoves && dto.validMoves.length > 0) ||
      AppState.Singleton.myColor.getValue() !== dto.currentPlayer;
  }

  ngOnDestroy(): void {
    this.gameSubs.unsubscribe();
  }
}

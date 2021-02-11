import { GameDto } from '../dto/gameDto';
import { StateObject } from './state-object';

export class AppState {
  constructor() {
    this.busy = new StateObject<boolean>();
    this.game = new StateObject<GameDto>();
  }
  private static _singleton: AppState;
  public static get Singleton(): AppState {
    if (!this._singleton) {
      this._singleton = new AppState();
    }
    return this._singleton;
  }

  busy: StateObject<boolean>;
  game: StateObject<GameDto>;
}

import { Component, Input, OnInit } from '@angular/core';
import { Observable } from 'rxjs';
import { AppStateService } from 'src/app/state/app-state.service';

@Component({
  selector: 'app-button',
  templateUrl: './button.component.html',
  styleUrls: ['./button.component.scss']
})
export class ButtonComponent implements OnInit {
  @Input() default = false;
  @Input() type = 'button';
  @Input() small = false;
  constructor(private appState: AppStateService) {
    this.theme = this.appState.theme.observe();
  }

  ngOnInit(): void {
    setTimeout(() => {
      this.appState.theme.next();
    }, 1);
  }

  theme: Observable<string>;
}

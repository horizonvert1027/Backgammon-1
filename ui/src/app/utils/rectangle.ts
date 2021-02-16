export class Rectangle {
  constructor(
    public x: number,
    public y: number,
    public width: number,
    public height: number,
    public pointIdx: number
  ) {}

  set(
    x: number,
    y: number,
    width: number,
    height: number,
    pointIdx: number
  ): void {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    this.pointIdx = pointIdx;
  }

  public contains(x: number, y: number): boolean {
    return (
      x >= this.x &&
      x <= this.x + this.width &&
      y >= this.y &&
      y <= this.y + this.height
    );
  }

  draw(cx: CanvasRenderingContext2D): void {
    cx.strokeRect(this.x, this.y, this.width, this.height);
    cx.fillText(this.pointIdx.toString(), this.x, this.y);
  }

  hasValidMove = false;
  canBeMovedTo = false;
}

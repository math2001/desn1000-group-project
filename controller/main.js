"use strict";
function assert(cond, msg) {
    if (!cond)
        throw new Error("assertion error: " + (msg || ""));
}
function scalingFactor(ctx) {
    const width = ctx.canvas.width;
    const height = ctx.canvas.height;
    assert(width == height);
    return width;
}
function render(points, ctx) {
    const s = scalingFactor(ctx);
    const base = { x: 0, y: .30 };
    const L = .8;
    const max_distance = 300; // cm
    const min_distance = 0; // cm
    ctx.fillStyle = 'black';
    ctx.fillRect(0, 0, s, s);
    for (let point of points) {
        const radius = .01;
        const L = .45 * (point.distance / (max_distance - min_distance));
        const x = base.x + L * Math.sin(point.angle * Math.PI / 180);
        const y = base.y - L * Math.cos(point.angle * Math.PI / 180);
        ctx.beginPath();
        ctx.fillStyle = 'darkred';
        ctx.ellipse((x + .5) * s, (y + .5) * s, radius * s, radius * s, 0, 0, Math.PI * 2);
        ctx.fill();
    }
}
function main() {
    const canvas = document.querySelector('canvas');
    assert(!!canvas);
    const ctx = canvas.getContext('2d');
    assert(!!ctx);
    const ws = new WebSocket('ws://localhost:8080/ws');
    assert(!!ws);
    ws.addEventListener('open', () => {
        ws.send("terminal");
    });
    let points = [];
    ws.addEventListener('message', (e) => {
        const msg = e.data;
        const [angle, distance, timestamp] = msg.trim().split(' ').map(parseFloat);
        points.push({ angle, distance, timestamp });
    });
    const data = `-90 112 0
-84 165 1
-78 1 2
-72 110 3
-66 121 4
-60 230 5
-54 29 6
-48 47 7
-42 289 8
-36 18 9
-30 42 10
-24 149 11
-18 135 12
-12 159 13
-6 228 14
0 23 15
6 86 16
12 29 17
18 263 18
24 263 19
30 130 20
36 13 21
42 238 22
48 279 23
54 224 24
60 258 25
66 191 26
72 220 27
78 111 28
84 191 29
90 78 30`;
    points = data.split('\n').map(s => s.trim().split(' ').map(parseFloat)).map(([angle, distance, timestamp]) => { return { angle, distance, timestamp }; });
    points = points.map(({ angle, distance, timestamp }) => { return { angle, distance: 300, timestamp }; });
    render(points, ctx);
    console.log('render');
}
main();

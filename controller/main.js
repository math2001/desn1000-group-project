"use strict";
let clawAngleVal = 0;
const CLAW_ANGLE_MIN = 0;
const CLAW_ANGLE_MAX = 90;
const CLAW_ANGLE_DELTA = 15;
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
// modifies the points array
// removes the points that are too old
// invariant: time stamps are sorted
function removeOldPoints(points, config) {
    if (points.length <= 1)
        return;
    const now = points[points.length - 1].timestampMS;
    const threshold = now - config.remove_points_after_ms;
    let i = 0;
    while (i < points.length - 1 && points[i].timestampMS < threshold)
        i++;
    points.splice(0, i);
}
const mousePosition = { x: 0, y: 0 };
function getMousePos(canvas) {
    var rect = canvas.getBoundingClientRect();
    return {
        x: (mousePosition.x - rect.left) / (rect.right - rect.left) * rect.width,
        y: (mousePosition.y - rect.top) / (rect.bottom - rect.top) * rect.height
    };
}
const base = { x: 0, y: .40 };
const max_radius = .45;
let s = 0;
const max_distance = 50; // cm
const min_distance = 0; // cm
function assert_sorted(points) {
    return;
    for (let i = 0; i < points.length - 1; i++) {
        assert(points[i].timestampMS < points[i + 1].timestampMS, `${i}`);
    }
}
function render(points, ctx, config) {
    // assert_sorted(points)
    removeOldPoints(points, config);
    ctx.fillStyle = 'black';
    ctx.fillRect(0, 0, s, s);
    // render the arcs
    for (let val of [.25, .5, .75, 1]) {
        ctx.strokeStyle = 'white';
        ctx.beginPath();
        ctx.arc((base.x + .5) * s, (base.y + .5) * s, val * max_radius * s, Math.PI, 2 * Math.PI);
        ctx.stroke();
        ctx.fillStyle = 'white';
        ctx.fillText(`${(val * max_distance | 0)} cm`, (base.x + .5) * s + val * max_radius * s + 7, (base.y + .5) * s);
    }
    // render the lines
    for (let angle = -80; angle <= 80; angle += 20) {
        ctx.strokeStyle = 'white';
        ctx.beginPath();
        ctx.moveTo((base.x + .5) * s, (base.y + .5) * s);
        let dx = max_radius * s * Math.sin(angle * Math.PI / 180);
        let dy = -max_radius * s * Math.cos(angle * Math.PI / 180);
        ctx.lineTo((base.x + .5) * s + dx, (base.y + .5) * s + dy);
        ctx.stroke();
        ctx.save();
        if (dy < 0)
            ctx.textAlign = 'right';
        else
            ctx.textAlign = 'left';
        dx = (max_radius * s + 10) * Math.sin(angle * Math.PI / 180);
        dy = -(max_radius * s + 10) * Math.cos(angle * Math.PI / 180);
        ctx.translate((base.x + .5) * s + dx, (base.y + .5) * s + dy);
        ctx.rotate(Math.PI / 180 * angle);
        ctx.textAlign = 'center';
        ctx.fillText(`${angle} deg`, 0, 0);
        ctx.restore();
    }
    for (let point of points) {
        const radius = .01;
        const L = max_radius * (point.distance / (max_distance - min_distance));
        const x = base.x + L * Math.sin(point.angle * Math.PI / 180);
        const y = base.y - L * Math.cos(point.angle * Math.PI / 180);
        ctx.beginPath();
        ctx.fillStyle = 'red';
        ctx.ellipse((x + .5) * s, (y + .5) * s, radius * s, radius * s, 0, 0, Math.PI * 2);
        ctx.fill();
    }
    const mousePos = getMousePos(ctx.canvas);
    ctx.save();
    const dx = mousePos.x - (base.x + .5) * s;
    const dy = mousePos.y - (base.y + .5) * s;
    const distance = Math.round(Math.sqrt(dx * dx + dy * dy) / s / max_radius * max_distance);
    ctx.fillStyle = 'white';
    const angle = Math.round(Math.atan2(dy, dx) * 180 / Math.PI + 90);
    ctx.fillText(`${distance} cm, ${angle} deg`, mousePos.x, mousePos.y);
    ctx.restore();
    requestAnimationFrame(() => render(points, ctx, config));
}
function add_movement_controller(ws, slowMode, observe, clawAngle) {
    const FORWARD = 'f';
    const BACKWARDS = 'b';
    const TURN_LEFT = 'l';
    const TURN_RIGHT = 'r';
    const SLIDE_LEFT = 'L';
    const SLIDE_RIGHT = 'R';
    const STOP = '0';
    document.body.addEventListener('keydown', e => {
        // e.repeat doesn't work on firefox for some reason
        if (e.repeat) {
            return;
        }
        console.log(e.key);
        if (e.key == 'w') {
            ws.send(FORWARD);
        }
        else if (e.key == 's') {
            ws.send(BACKWARDS);
        }
        else if (e.key == 'a') {
            ws.send(SLIDE_LEFT);
        }
        else if (e.key == 'd') {
            ws.send(SLIDE_RIGHT);
        }
        else if (e.key == 'ArrowRight') {
            ws.send(TURN_RIGHT);
        }
        else if (e.key == 'ArrowLeft') {
            ws.send(TURN_LEFT);
        }
        else if (e.key == '0' || e.key == '1' || e.key == '2' || e.key == '3' || e.key == '4' || e.key == '5' || e.key == '6' || e.key == '7' || e.key == '8' || e.key == '9') {
            ws.send(e.key);
        }
        else if (e.key == 'm') {
            slowMode.checked = !slowMode.checked;
            slowMode.dispatchEvent(new Event('change'));
        }
        else if (e.key == 'o') {
            observe.checked = !observe.checked;
            observe.dispatchEvent(new Event('change'));
        }
        else if (e.key == 'c') {
            clawAngleVal -= CLAW_ANGLE_DELTA;
            if (clawAngleVal < CLAW_ANGLE_MIN) {
                clawAngleVal = CLAW_ANGLE_MIN;
            }
            ws.send("c" + (clawAngleVal.toString()) + ".");
            clawAngle.textContent = clawAngleVal.toString();
        }
        else if (e.key == 'C') {
            clawAngleVal += CLAW_ANGLE_DELTA;
            if (CLAW_ANGLE_MAX < clawAngleVal) {
                clawAngleVal = CLAW_ANGLE_MAX;
            }
            ws.send("c" + (clawAngleVal.toString()) + ".");
            clawAngle.textContent = clawAngleVal.toString();
        }
        else if (e.key == 'i') {
            ws.send('i');
        }
        else if (e.key == 'k') {
            ws.send('k');
        }
    });
    document.body.addEventListener('keyup', e => {
        if (e.key == 'w') {
            ws.send(STOP);
        }
        else if (e.key == 's') {
            ws.send(STOP);
        }
        else if (e.key == 'a') {
            ws.send(STOP);
        }
        else if (e.key == 'd') {
            ws.send(STOP);
        }
        else if (e.key == 'ArrowRight') {
            ws.send(STOP);
        }
        else if (e.key == 'ArrowLeft') {
            ws.send(STOP);
        }
    });
}
function main() {
    document.body.addEventListener('mousemove', e => {
        mousePosition.x = e.clientX;
        mousePosition.y = e.clientY;
    });
    const canvas = document.querySelector('canvas');
    assert(!!canvas);
    const ctx = canvas.getContext('2d');
    assert(!!ctx);
    const slowMode = document.querySelector("#slow-mode");
    assert(!!slowMode);
    const observe = document.querySelector("#scanning");
    assert(!!observe);
    s = scalingFactor(ctx);
    const clawAngle = document.querySelector("#claw-angle");
    assert(!!clawAngle);
    const ws = new WebSocket('ws://localhost:8080/ws');
    assert(!!ws);
    ws.addEventListener('open', () => {
    });
    canvas.addEventListener('click', e => {
        const mousePos = getMousePos(canvas);
        const dx = mousePos.x - (base.x + .5) * s;
        const dy = mousePos.y - (base.y + .5) * s;
        if (dy > 0)
            return;
        const angle = Math.round(Math.atan2(dy, dx) * 180 / Math.PI + 90);
        const sending_angle = 180 - (angle + 90);
        console.log(angle, sending_angle);
        // const buf = new Uint8Array(1)
        // buf[0] = sending_angle
        // ws.send(buf)
        ws.send("s" + (sending_angle.toString()) + ".");
    });
    slowMode.addEventListener('change', e => {
        ws.send('m');
    });
    observe.addEventListener('change', () => {
        ws.send('o');
    });
    add_movement_controller(ws, slowMode, observe, clawAngle);
    let points = [];
    ws.addEventListener('message', (e) => {
        const msg = e.data;
        if (msg.startsWith(';')) {
            if (msg == '; reset') {
                slowMode.checked = false;
                observe.checked = false;
                slowMode.dispatchEvent(new Event('change'));
                observe.dispatchEvent(new Event('change'));
                clawAngle.textContent = '0';
                clawAngleVal = 0;
                points.splice(0, points.length);
            }
            console.info("Received comment", msg);
            return;
        }
        let [angle, distance, timestampMS] = msg.trim().split(' ').map(parseFloat);
        // distance = 100
        angle *= -1;
        points.push({ angle, distance, timestampMS });
        // console.log("new point", msg, points.length)
    });
    clawAngle.textContent = clawAngleVal.toString();
    // points = data.split('\n').map(s => s.trim().split(' ').map(parseFloat)).map(([angle, distance, timestampMS]) => {return {angle, distance, timestampMS}})
    // points = points.map(({angle, distance, timestampMS}) => { return {angle, distance: 300, timestampMS}})
    render(points, ctx, config);
}
const config = { remove_points_after_ms: 2 * 1000 };
document.addEventListener("DOMContentLoaded", () => {
    main();
});

// Mock for MorphTimer class
export class MorphTimer {
    constructor() {
        this.timers = new Map();
        this.nextId = 1;
        console.log('MorphTimer mock created');
    }

    setTimeout(callback, delayMS) {
        const id = this.nextId++;
        const timer = {
            id,
            callback,
            delay: delayMS,
            startTime: Date.now()
        };
        
        this.timers.set(id, timer);
        
        // 在Node.js环境中使用真实的setTimeout
        const realTimeout = setTimeout(() => {
            this.timers.delete(id);
            callback();
        }, delayMS);
        
        timer.realTimeout = realTimeout;
        return id;
    }

    clearTimeout(id) {
        const timer = this.timers.get(id);
        if (timer && timer.realTimeout) {
            clearTimeout(timer.realTimeout);
            this.timers.delete(id);
            console.log(`Timer ${id} cleared`);
        }
    }

    setImmediate(fn, ...args) {
        return this.setTimeout(() => fn(...args), 0);
    }
}

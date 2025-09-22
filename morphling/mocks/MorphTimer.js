// Mock for MorphTimer class
export class MorphTimer {
    constructor() {
        this.timers = new Map();
        this.nextId = 1;
        console.log('MorphTimer mock created');
        
        // 保存原始的setTimeout和clearTimeout
        this.originalSetTimeout = (typeof globalThis !== 'undefined' && globalThis.originalSetTimeout) || 
                                 (typeof global !== 'undefined' && global.originalSetTimeout) || 
                                 (typeof window !== 'undefined' && window.originalSetTimeout) || 
                                 setTimeout;
        this.originalClearTimeout = (typeof globalThis !== 'undefined' && globalThis.originalClearTimeout) || 
                                   (typeof global !== 'undefined' && global.originalClearTimeout) || 
                                   (typeof window !== 'undefined' && window.originalClearTimeout) || 
                                   clearTimeout;
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
        
        // 使用原始的setTimeout，避免循环调用
        const realTimeout = this.originalSetTimeout(() => {
            this.timers.delete(id);
            callback();
        }, delayMS);
        
        timer.realTimeout = realTimeout;
        return id;
    }

    clearTimeout(id) {
        const timer = this.timers.get(id);
        if (timer && timer.realTimeout) {
            this.originalClearTimeout(timer.realTimeout);
            this.timers.delete(id);
            console.log(`Timer ${id} cleared`);
        }
    }

    setImmediate(fn, ...args) {
        return this.setTimeout(() => fn(...args), 0);
    }
}

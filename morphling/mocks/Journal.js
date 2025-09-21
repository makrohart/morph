// Mock for Journal class
export class Journal {
    constructor() {
        console.log('Journal mock created');
    }

    log(message, ...args) {
        console.log(`[Journal] ${message}`, ...args);
    }

    warn(message, ...args) {
        console.warn(`[Journal] ${message}`, ...args);
    }

    error(message, ...args) {
        console.error(`[Journal] ${message}`, ...args);
    }

    info(message, ...args) {
        console.info(`[Journal] ${message}`, ...args);
    }
}

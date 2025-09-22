// Mock for Journal class
export class Journal {
    constructor() {
        // 使用原始的console.log，避免循环调用
        if (typeof globalThis !== 'undefined' && globalThis.originalConsole) {
            globalThis.originalConsole.log('Journal mock created');
        } else {
            console.log('Journal mock created');
        }
    }

    log(message, ...args) {
        // 使用原始的console.log，避免循环调用
        if (typeof globalThis !== 'undefined' && globalThis.originalConsole) {
            globalThis.originalConsole.log(`[Journal] ${message}`, ...args);
        } else {
            console.log(`[Journal] ${message}`, ...args);
        }
    }

    warn(message, ...args) {
        if (typeof globalThis !== 'undefined' && globalThis.originalConsole) {
            globalThis.originalConsole.warn(`[Journal] ${message}`, ...args);
        } else {
            console.warn(`[Journal] ${message}`, ...args);
        }
    }

    error(message, ...args) {
        if (typeof globalThis !== 'undefined' && globalThis.originalConsole) {
            globalThis.originalConsole.error(`[Journal] ${message}`, ...args);
        } else {
            console.error(`[Journal] ${message}`, ...args);
        }
    }

    info(message, ...args) {
        if (typeof globalThis !== 'undefined' && globalThis.originalConsole) {
            globalThis.originalConsole.info(`[Journal] ${message}`, ...args);
        } else {
            console.info(`[Journal] ${message}`, ...args);
        }
    }
}

import { View } from './View.js';

export class WindowView extends View {
    constructor() {
        super();
        this.type = 'window';
        console.log('WindowView created');
    }
}

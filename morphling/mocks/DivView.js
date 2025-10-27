import { View } from './View.js';

export class DivView extends View {
    constructor() {
        super();
        this.type = 'div';
        console.log('DivView created');
    }
}

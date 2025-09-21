import { View } from './View.js';

export class ButtonView extends View {
    constructor() {
        super();
        this.type = 'button';
        console.log('ButtonView created');
    }
}

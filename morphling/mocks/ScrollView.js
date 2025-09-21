import { View } from './View.js';

export class ScrollView extends View {
    constructor() {
        super();
        this.type = 'scroll';
        console.log('ScrollView created');
    }
}

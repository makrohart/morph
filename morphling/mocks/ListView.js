import { View } from './View.js';

export class ListView extends View {
    constructor() {
        super();
        this.type = 'list';
        console.log('ListView created');
    }
}

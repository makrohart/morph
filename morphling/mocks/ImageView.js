import { View } from './View.js';

export class ImageView extends View {
    constructor() {
        super();
        this.type = 'image';
        console.log('ImageView created');
    }
}

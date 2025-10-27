import { View } from './View.js';

export class TextView extends View {
    constructor(text = '') {
        super();
        this.type = 'text';
        this.text = text;
        console.log('TextView created with text:', text);
    }
    
    // 模拟原生setText方法
    setText(text) {
        this.text = text;
        console.log('TextView setText called with:', text);
    }
}

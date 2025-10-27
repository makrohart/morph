import { View } from './View.js';

export class ButtonView extends View {
    constructor() {
        super();
        this.type = 'button';
        this.text = ''; // 添加文本属性
        console.log('ButtonView created');
    }
    
    // 重写setProperty方法以处理文本更新
    setProperty(property, value) {
        if (property === 'text') {
            this.text = value;
            console.log(`Button text updated to: ${value}`);
        } else {
            super.setProperty(property, value);
        }
    }
}

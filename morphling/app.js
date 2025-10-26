import React from 'react';

function App() {
    console.log("App function called - component is rendering");
    
    // 简单的状态管理
    const [count, setCount] = React.useState(0);
    
    // 使用 useCallback 来稳定函数引用，避免每次渲染都创建新的函数
    const handleClick = React.useCallback(() => {
        console.log("Clicked! Background color changed!");
        setCount(prevCount => {
            const newCount = prevCount + 1;
            console.log(`setCount 被触发，新计数: ${newCount}`);
            return newCount;
        });
    }, []); // 空依赖数组，函数引用不会改变

    return React.createElement(
        "window",
        {
            style: {
                width: '1000',
                height: '600',
            }
        },
        React.createElement(
            "div", 
            {
                style: {
                    alignItems: '8',
                    justifyContent: '5',
                    flexDirection: '3',
                }
            },
            [
                React.createElement(
                    "button",
                    {
                        key: "main-button",
                        onClick: handleClick,
                        style: {
                            widthPercent: "50",
                            heightPercent: "50",
                        },
                    },
                    `Click Me (${count})`
                ),
                React.createElement(
                    "window",
                    {
                        key: "sub-window",
                        style: {
                            width: '500',
                            height: '300',
                        }
                    }
                )
            ]
        )  
    );
}

export { App };
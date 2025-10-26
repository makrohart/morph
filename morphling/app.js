import React from 'react';

function App() {
    console.log("App function called - component is rendering");
    
    // 简单的状态管理
    const [count, setCount] = React.useState(0);
    
    // Use useEffect to log when component mounts and trigger setCount
    React.useEffect(() => {
        console.log("App component mounted - useEffect called");
        
        // 渲染完成后自动触发 setCount
        setTimeout(() => {
            console.log("自动触发 setCount...");
            setCount(prevCount => {
                const newCount = prevCount + 1;
                console.log(`setCount 被触发，新计数: ${newCount}`);
                return newCount;
            });
        }, 100); // 延迟100ms确保渲染完成
    }, []);

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
                        onClick: () => { 
                            console.log("Clicked! Background color changed!");
                            setCount(count + 1);
                            console.log(`Click count: ${count}`);
                        },
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
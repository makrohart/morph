import React from 'react';

function App() {
    console.log("App function called - component is rendering");
    
    // 简单的状态管理
    const [count, setCount] = React.useState(0);
    
    // Use useEffect to log when component mounts
    React.useEffect(() => {
        console.log("App component mounted - useEffect called");
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
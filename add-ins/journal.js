var a = new A();
a.run(10);
var journal = new Journal
if (typeof React !== 'undefined') {
  journal.log("React loaded");
} else {
  journal.log("React not loaded");
}

if (ReactDOM && ReactDOM.version) {
  journal.log(`ReactDOM version: ${ReactDOM.version}`);
} else {
  journal.log('ReactDOM not loaded');
}

var viewModel = new ViewModel();
viewModel.onPropertyChanged("ViewModelPropertyChanged", () => { journal.log("ViewModelProperyChanged event raise")});
viewModel.onPropertyChanged("IntField", () => { journal.log("IntField changed")});
viewModel.notifyPropertyChanged("ViewModelPropertyChanged");
journal.log(viewModel.IntField);
journal.log(viewModel.StringField);
viewModel.addInt();


const { useState, useEffect } = React;

function useViewModel(vmClass) {
  // 1. 实例化 ViewModel（确保单例）
  const [vm] = useState(() => new vmClass());

  // 2. 初始化状态（通过 .get() 读取属性值）
  const [state, setState] = useState(() => {
    const initialState = {};
    for (const key in vm) {
      if (vm[key] && typeof vm[key].get === 'function') { // 识别 Property
        initialState[key] = vm[key].get();
      }
    }
    return initialState;
  });

  // 3. 注册 C++ 回调：属性变更时更新 JS 状态
  useEffect(() => {
    const cleanupCallbacks = [];

    for (const key in state) {
      if (vm[key] && typeof vm[key].get === 'function') {
        const unsubscribe = vm.onPropertyChanged(
          key,
          () => {
            // 重要：直接从 C++ 读取最新值，避免状态不一致
            setState(prev => ({ ...prev, [key]: vm[key].get() }));
          }
        );

        if (typeof unsubscribe === 'function') {
          cleanupCallbacks.push(unsubscribe);
        }
      }
    }

    return () => cleanupCallbacks.forEach(fn => fn());
  }, [vm]);

  // 4. 创建代理对象（处理 JS → C++ 的更新）
  const proxyState = new Proxy({ ...state }, {
    set: (target, key, value) => {
      // 仅处理存在于初始 state 的 key
      if (key in state) {
        if (vm[key] && typeof vm[key].set === 'function') {
          vm[key].set(value); // 先更新 C++
        }
        target[key] = value;   // 再更新代理对象
        setState(prev => ({ ...prev, [key]: value })); // 触发渲染
      }
      return true;
    }
  });

  // 5. 绑定命令
  const commands = {};
  // for (const key in vm) {
  //   if (vm[key] && typeof vm[key].execute === 'function') {
  //     commands[key] = (...args) => vm[key].execute(...args);
  //   }
  // }

  return { ...proxyState, ...commands };
}

// function App() {
//   return (
//     <div>
//       <h1>Hello</h1>
//       <button onClick={() => console.log("Clicked!")}>Click Me</button>
//     </div>
//   );
// }

function App() {
  journal.log("App function called - component is rendering");
  
  // Use useEffect to log when component mounts
  React.useEffect(() => {
    journal.log("App component mounted - useEffect called");
  }, []);

  return React.createElement(
    "div",
    null,
    React.createElement("h1", null, "Hello"),
    React.createElement(
      "button",
      { onClick: () => journal.log("Clicked!") },
      "Click Me"
    )
  );
}

const container = {};
journal.log("About to render App component");
render(React.createElement(App), container);
journal.log("Render call completed");


var node = new MorphNode();
var node1 = new MorphNode();
node.add(node1);
node.remove(node1);
## Release 模式下开启 Jvmti

代码主要在 https://github.com/PTrain666/JVMTI_RELEASE_DEMO/blob/master/jvmti/src/main/cpp/jvmti_hook.cpp

大部分借鉴 https://juejin.cn/post/6942782366993612813 这篇文章

核心是调用 libart.so 的 SetJdwpAllowed() setJavaDebuggable() 方法，来突破限制
***
**注意**
SetJdwpAllowed() setJavaDebuggable() 这俩方法不要修改，会有安全风险

可以直接手动加载一些 so

## 如何调用 so 私有方法

可以通过 dlopen，再通过 dlsym 找到对应的函数的入口地址，强转调用即可  

其中：
```txt
SetJdwpAllowed -> _ZN3art3Dbg14SetJdwpAllowedEb  

setJavaDebuggable -> _ZN3art7Runtime17SetJavaDebuggableEb
```

不过，高版本 dlopen 被限制了，原文中直接使用 inline hook，这里使用的是 `xdl` 突破 dlopen 限制

一个小问题是 setJavaDebuggable 方法需要一个 Runtime 类型参数，我这里没有细看是不是 Java 层的对象，所以干脆也直接用 dlsym 调用了

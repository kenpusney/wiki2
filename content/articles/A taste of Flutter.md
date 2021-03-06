---
title: 说说 Flutter：Skia、Dart 及其历史
date: 2020-05-29
---
最近因为工作需要，上手了用了用 Flutter。恰好加上前段时间空的时候糊了一些 C++ 的 2D 图形处理和渲染的代码，感触挺深。这里就简单聊一下，关于 Flutter、Dart 及其渲染引擎 Skia 相关的事情。

## 跨平台 UI 框架

Flutter 是一个跨平台 UI 框架。与之前基于 WebView/Native 的框架不同，Flutter 的 UI 呈现与平台完全无关，所有的图形渲染工作都是通过 Skia 这个引擎来实现。

这样做的一个非常重要的优点就是一致性的 UI 体验。传统的基于 WebView 和 Native 方法的跨平台框架，都必须强行忍受不同平台控件的兼容性，或者通过额外的样式代码来调整不同平台间的样式，使其保持一致。Flutter 则直接把这些问题全部通过 Skia 这一步操作给去掉了。所有 Flutter 的控件，跟平台完全无关，最终呈现出来的结果只是在 Skia 引擎上展现的 2D 图形，再由 Skia 渲染到对应的图像处理后端（比如 OpenGL）。

举一个容易理解的例子，常规的 WebView/Native 开发，像是我们在浏览器里用各种 HTML 控件和 CSS 来控制样式，而使用了 Flutter 之后，就变成了在一个`<canvas />`元素上直接画出来我们想要的效果就好了。平台可能对不同的 HTML 控件显示不一致，但是所有实现了 Canvas 的平台都能正常渲染 2D 图形。这时候把渲染就变成了平台无关的。

## 跨平台的运行时

助力 Flutter 跨平台的另外一个组件就是 Dart VM，跨平台的运行时。

早在 Dart 第一版的时候我就已经试用过了。当时便即断言，在 JS Alternative 中，同属于静态类型语言的 Dart 和 TypeScript，必定是后者胜出。Dart 1 给人的感觉是简陋和粗糙，并且带着不明确的目的。当时吐槽比较多的就是，一个 Dart 版的 hello world，编译到 JS 之后会得到一个非常大的 bundle，这点明显就给从 JavaScript 转移到 Dart 带来了很大的阻力。而其实后面 Flutter 出来以后也看出来了这一点，Dart 的存在只是另外一个造 App 的工具，并没有取代 JavaScript 成为 Web 通用语言的目的。现在的 Dart 甚至有专心为 Flutter 打工的意思在了。

另外一个比较有意思的点是，无论早期的 Dart 还是 Dart 2，设计上都是受 Java 的影响比较大。其原因跟我们接下来要说的历史渊源有着很重要关系。

## Google Web Toolkit

估计已经有很多人没听过 RIA (Rich Internet Application)这个术语了，早在 2006 年，YUI 带起来的一波开源框架逐渐成为互联网应用开发的主流，这其中包括现在还没死透的 dojo、ExtJS 等。这些框架在充分利用了 JavaScript 对象系统的情况下实现了丰富的控件、布局和交互系统，让 JavaScript 有了开发复杂业务应用的能力。来自谷歌的 Google Web Toolkit (GWT) 也是其中一员。

与 dojo 之流还在想尽办法扩展 JavaScript 的思路截然不同，GWT 主要编程语言是 Java。所有的控件也都是 Java 对象，在经过布局引擎的处理以后，编译到浏览器前端。除了做事情的工序复杂了点，整体上这套操作是有好处的：业务逻辑由 Java 写成，能够充分利用 Java 的强类型特性；最终生成的 JavaScript 代码，也可以经由编译器做一些深度优化。要知道那个时候 Chrome (v8) 还没出现，Spidermonkey 虽然会做 JIT，可 Firefox 还没有足够的市场优势，无论如何都要考虑 IE 这个迟钝的家伙。

所以经由 GWT 开发的 Web 应用有一个显著的特点，就是在前端几乎不可逆，因为大量渲染和优化之后的结果都是自动生成的，跟背后的 Java 逻辑直接关联性已经不明显了。

按照 Google 员工 David Chandler 的说法，Google 在 2011 年的时候已经有数百个项目使用 GWT 重写过了，其中还包含一些重量级项目比如 Blogger、Map、AdWords 和 Inbox 等。

## Flutter

然而也就是在 2011 年，Google 公布了 Dart，这其中就有一部分人，从 GWT 项目转到了 Dart 上去。两年后 GWT 从 Google 的项目转成了一个完全开源的项目。

而去了 Dart 的人，也就是把 Java-to-JavaScript 改成 Dart-to-JavaScript。这就不难理解 Dart 跟 Java 如此相似的原因了。特别是做到语法级兼容的话，改一下扩展名就能用多好（D 语言社区干过这样的事儿，因为语法在一定程度上与 Java 相似性很高，直接搬过来了 Eclipse SWT 的代码，修修补补作为自己的 UI 框架用）。这样其实也就能理解为什么当年的 Dart 能编译到 JavaScript，并且带着一个庞大的运行时了。

至于 Flutter 呢，整个的 UI 框架逻辑其实跟 Android 相去甚远，整个继承树反倒是跟 GWT 的一致性非常高。其实想想也应该可以理解，因为逻辑上 Android UI 是基于 Java 内建的，并且本身有着很多系统服务和工具在，自然有一套自身的体系在。Flutter 和 GWT 都是作为 UI 框架有一个编译到特定平台的过程，其功能和应用场景的相似性反倒是更高一些。

并且，本身 Dart 生态不够丰富的情况下，直接做一个 source to source 的替换和翻译，就能先把 GWT 的代码一次性拿到手用着。这样 Flutter 快速成型，那再谈改进的事儿就简单多了。

## Adopting Flutter: 怎么引入？

这里额外聊一下切换到 Flutter 技术栈的问题。

首先是新技术栈。这会是在团队推进关键阻碍。更尴尬的情况是，Dart 语言目前只此一家在用，对于开发者来说并不算实惠。好在进入的成本并不高，Algol 系的编程语言都有着一脉相承的结构。这点上来说，还是要看团队成员的能力和接受程度来适当推行。

其次是工具支持。官方给了足够丰富的工具支撑，IDE 方面，可以直接用 Android Studio 或者 IntelliJ IDEA，同时在 Visual Studio Code 上提供了插件。Google 本身比较注重工程实践能力，所以周边工具上也给予完整的支持。在 Android Studio/IntelliJ 版本里面，各种重构功能也完全能够满足，完全可以跟 JetBrains 的亲儿子（Kotlin）媲美。

再者是生态和社区支持。Flutter 已经是正式版发布，稳定性有足够的保障。并且作为 Fuchsia 的默认 UI 框架，Google 在技术上的投入也足够，官方的文档和生态支持相对完善。Dart 提供了专门的 FFI 接口，能够方便扩展 Flutter 没有而需要在对应平台上实现的功能，这一点可以让团队去复用之前的逻辑。并且 Dart/Flutter 也在维护自己的包管理系统，并且收录了很多第三方的扩展。这也能给 Flutter 的生态添加足够强的支撑。

关于具体的平台（Android / iOS）如何逐步引入 Flutter，官方文档有详细的介绍，可以参考。

## 题外话

关于 Flutter 的主要话题大概就到这儿了。这里扯几点题外话。

如果你了解 golang 的话，再对比下如今的 Dart/Flutter，就会发现，Google 造的轮子，最重要的仍然是自身的需求。像 golang/Dart 这些被反复吐槽和诟病、有一些明显设计缺陷的编程语言，Google 也会毫不顾忌地拿出来。这种时候如果你的场景类似并且这个轮子刚搞够用，那自然还好。否则可能会痛苦的多。

当然也不是说轮子非得开放到不停改进和演化，每年重学一遍编程语言的结果也挺累的。

以上。

<p align="center">
  <img src="docs/art.png" />
</p>

# 作业 7：Unique Pointer

截止时间：5 月 31 日（周日）晚上 11:59

## 概述

在本作业中，你将实现一个自定义版本的 `unique_ptr`，以加深对本周课程中介绍的 RAII 和智能指针等概念的理解。此外，你还将练习本学期学到的多项技能：模板、运算符重载和移动语义。

你将在此作业中处理以下三个文件：

- `unique_ptr.h` — 包含 `unique_ptr` 实现的所有代码。
- `main.cpp` — 包含使用 `unique_ptr` 的代码。你需要在这里编写一个函数！
- `short_answer.txt` — 包含一些简答题，你需要在完成作业的过程中回答。

## 运行代码

要运行代码，首先需要编译它。打开终端（如果使用 VSCode，按 <kbd>Ctrl+\`</kbd> 或前往 **Terminal > New Terminal**）。然后确保你在 `assign7/` 目录下，运行：

```sh
g++ -std=c++20 main.cpp -o main
```

假设代码编译没有错误，接下来可以运行：

```sh
./main
```

这将实际执行 `main.cpp` 中的 `main` 函数。

在按照以下说明操作时，我们建议间歇性地编译并用自动评分器测试，以确保你走在正确的轨道上！

> [!NOTE]
>
> ### Windows 用户注意
>
> 在 Windows 上，你可能需要使用以下命令编译代码：
>
> ```sh
> g++ -static-libstdc++ -std=c++20 main.cpp -o main
> ```
>
> 才能看到输出。此外，生成的可执行文件可能名为 `main.exe`，此时你需要通过以下命令运行代码：
>
> ```sh
> ./main.exe
> ```

## 第 1 部分：实现 `unique_ptr`

在本部分中，你将实现我们在周四课堂上讨论过的一种智能指针：`unique_ptr`。你要实现的 `unique_ptr` 是标准库 [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr) 的简化版本。回顾一下，`unique_ptr` 是指向动态分配内存的指针，该内存由一个（*唯一*）变量拥有。当该变量离开作用域时，它会自动调用 `delete` 清理其所拥有的分配内存。这种行为称为 RAII（资源获取即初始化）。**在本作业中，你可以假设 `unique_ptr` 指向单个类型为 T 的元素。你无需在任何时候调用 `delete[]` 或处理指向动态分配数组的指针。**

> [!IMPORTANT]  
> ##### `short_answer.txt`  
> **Q1：** 列出使用 RAII 管理内存相比手动调用 `new` 和 `delete` 的一到两个好处。

> [!NOTE]
> 虽然我们的 `unique_ptr` 不支持指向数组的指针，但如果我们愿意，可以添加此行为。例如，C++ 标准库 `std::unique_ptr` 使用 *模板特化* 为数组指针实现不同的行为。这样的模板特化可能如下所示：
>
> ```cpp
> template <typename T>
> class unique_ptr<T[]>;
> ```
>
> 实际上，我们会有两个版本的 `unique_ptr`：一个用于单个元素，一个用于元素数组。每个版本支持不同的操作；例如，数组版本提供下标运算符（`operator[]`）来访问数组中的元素，而单个元素版本则不提供。

### 实现 `unique_ptr` 功能

花点时间浏览 `unique_ptr.h` 中提供的 `unique_ptr` 代码。我们提供了 `unique_ptr` 的基本接口，你需要实现这个接口。请记住，`unique_ptr` 应该看起来和行为像普通指针一样，支持解引用（`operator*`）和成员访问（`operator->`）等操作。其中几个方法同时有 `const` 和非 `const` 版本，以使我们的类完全符合 const 正确性要求。

你需要通过实现以下要点来完成 `unique_ptr` 的基本指针接口。每项任务应该相对直接，只需在 `unique_ptr.h` 中增加/修改 1-2 行代码即可完成：

* `unique_ptr` 的 `private` 部分
* `unique_ptr(T* ptr)`（构造函数）
* `unique_ptr(std::nullptr_t)`（针对 `nullptr` 的构造函数）
* `T& operator*()`
* `const T& operator*() const`
* `T* operator->()`
* `const T* operator->() const`
* `operator bool() const`

### 实现 RAII

到目前为止，我们的 `unique_ptr` 的行为类似于原始指针，但它实际上不会执行任何自动内存管理，例如在 `unique_ptr` 变量离开作用域时释放内存。此外，我们的指针并不是*唯一的*：可以不加区分地创建它的多个副本（都指向同一块内存）。例如，假设我们的 `unique_ptr` 在离开作用域时能正确清理其数据，考虑以下代码块：

```cpp
int main() 
{
  unique_ptr<int> ptr1 = make_unique<int>(5);

  // ptr1 指向 5（在堆上动态分配）

  {

    unique_ptr<int> ptr2 = ptr1; // 浅拷贝

  } // <-- ptr2 的数据在此处被释放

  std::cout << *ptr1 << std::endl;
  return 0;
}
```

由于 `ptr1` 和 `ptr2` 指向同一块内存，当 `ptr2` 离开作用域时，它也带走了 `ptr1` 的数据！因此，`*ptr1` 的行为是未定义的。

另一方面，我们仍然应该能够**移动** `unique_ptr`。回顾一下，移动语义允许我们获取对象资源的所有权，而无需进行昂贵的拷贝。移动 unique pointer 是有效的，因为它保持了指针的唯一性——在任何时间点，我们仍然只有一个指针指向底层内存。我们只是改变了谁（哪个变量）拥有该内存。

为了实现这些目标——自动释放内存、禁止拷贝和移动语义——我们需要在 `unique_ptr` 类上实现一些特殊的成员函数。**具体来说，需要实现以下 SMF（特殊成员函数）：**

* `~unique_ptr()`：释放指针的内存
* `unique_ptr(const unique_ptr& other)`：拷贝 unique pointer。应被删除。
* `unique_ptr& operator=(const unique_ptr& other)`：拷贝赋值 unique pointer。应被删除。
* `unique_ptr(unique_ptr&& other)`：移动 unique pointer。
* `unique_ptr& operator=(unique_ptr&& other)`：移动赋值 unique pointer。

实现上述函数后，你应该能通过自动评分器中**第 1 部分**的所有测试。

> [!IMPORTANT]  
> ##### `short_answer.txt`  
> **Q2：** 在为 `unique_ptr` 实现移动语义时，例如在移动构造函数 `unique_ptr(unique_ptr&& other)` 中，至关重要的是在退出函数前将 `other` 参数的底层指针设置为 `nullptr`。请用你自己的话解释如果不这样做会出现什么问题。

## 第 2 部分：使用 `unique_ptr`

现在我们有了 `unique_ptr` 的实现，来使用它吧！查看 `main.cpp`。我们为你提供了一个完整的单向链表（`ListNode`）实现，它利用 `unique_ptr` 确保链表中的所有节点都能正确释放。例如，以下代码会产生这样的输出：

```cpp
int main()
{

  auto head = cs106l::make_unique<ListNode<int>>(1);
  head->next = cs106l::make_unique<ListNode<int>>(2);
  head->next->next = cs106l::make_unique<ListNode<int>>(3);

  // head 的内存布局：
  //
  // head -> (1) -> (2) -> (3) -> nullptr
  //
  //

} // <- `head` 在此处被析构！

// 输出：
// Constructing node with value '1'
// Constructing node with value '2'
// Constructing node with value '3'
// Destructing node with value '1'
// Destructing node with value '2'
// Destructing node with value '3'
```

注意我们不需要调用任何 `delete`！`unique_ptr` 的 RAII 行为保证了链表中的所有内存都会被递归释放。当 `head` 离开作用域时，它会调用节点 `(1)` 的析构函数，进而调用 `(2)` 的析构函数，再进而调用 `(3)` 的析构函数。

> [!IMPORTANT]  
> ##### `short_answer.txt`  
> **Q3：** 这种通过 RAII 递归释放内存的方法对于小型链表效果很好，但对于较长的链表可能会带来问题。为什么？提示：递归函数的调用栈能增长多"深"的限制是什么？

**你的任务是实现 `create_list` 函数，该函数将 `std::vector<T>` 转换为 `unique_ptr<ListNode<T>>`。** 向量中元素的顺序应在链表中保持不变，对于空向量应返回 `nullptr`。有多种方法可以实现；一种方法是反向构造链表（从尾节点开始逐步构建到头节点）。**注意，你必须使用 `cs106l` 命名空间下的 `cs106l::unique_ptr`，而不是 `std::unique_ptr`！** 以下是你在实现中应遵循的算法：

1. 初始化 `cs106l::unique_ptr<ListNode<T>> head = nullptr`。
2. **反向**遍历 `std::vector`。对于向量中的每个元素：
    - 2a. 创建一个新的 `cs106l::unique_ptr<ListNode<T>> node`，其值为向量中的该元素。
    - 2b. 将 `node->next` 设置为 `head`。
    - 2c. 将 `head` 设置为 `node`。
3. 最后返回 `head`。

> [!IMPORTANT]  
> ##### `short_answer.txt`  
> **Q4.** 在实现步骤 2b 和 2c 时，你可能很难让编译器允许你赋值，例如将 `head` 赋值给 `node->next`，因为编译器会报错说没有拷贝赋值运算符。这正是我们预期的，因为正如之前讨论的，`unique_ptr` 不能被拷贝！
>
> 为了获得我们想要的行为，我们必须强制编译器对 `head` 执行**移动赋值**到 `node->next`，而不是拷贝赋值。回顾移动语义讲座，我们可以通过编写 `node->next = std::move(head)` 来实现这一点。
>
> 在这个上下文中，`std::move` 做了什么？为什么在这里使用 `std::move` 和移动语义是安全的？

> [!NOTE]  
> 在反向遍历向量时注意不要使用 `size_t` 作为索引。`size_t` 只能是非负整数，在检查 for 循环边界时试图降到零以下会导致意外行为。
> 为了解决这个问题，请尝试使用 `int`。

一旦你实现了 `create_list`，我们就可以创建链表并打印它。作为额外加分，请查看 `map_list()` 和 `linked_list_example()` 函数，它们共同调用了你的 `create_list` 函数并将每个元素打印在单独的一行上。此时，你应该通过**第 2 部分**中的所有测试。

## 🚀 提交说明

如果你通过了所有测试，就可以提交了！提交作业的步骤：
1. 请填写[此链接](https://forms.gle/uHr3J8Vm3gECkZpm9)的反馈表。
2. 在 [Paperless](https://paperless.stanford.edu) 上提交你的作业！

你需要提交的文件：

- `unique_ptr.h`
- `main.cpp`
- `short_answer.txt`

你可以在截止日期前多次重新提交。

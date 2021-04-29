from tkinter import *
from tkinter.filedialog import askdirectory

def selectPath():
    path_ = askdirectory()
    path.set(path_)
root = Toplevel()

path = StringVar()
Label(root,text = "目标路径:").grid(row = 0, column = 0)
Entry(root, textvariable = path).grid(row = 0, column = 1)
Button(root, text = "路径选择", command = selectPath).grid(row = 0, column = 2)

filename = StringVar()
Label(root,text = "文件命名为：").grid(row = 1, column = 0)
Entry(root, textvariable = filename).grid(row = 1, column = 1)
Button(root, text = "确定", command = root.quit).grid(row = 2, column = 1)
root.mainloop()
print(path.get())
print(filename.get())
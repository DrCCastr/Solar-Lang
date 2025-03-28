; ModuleID = 'Main'
source_filename = "Main"

define i32 @idk(i32 %b, i32 %a) {
entry:
  %addtmp = add i32 %a, %b
  ret i32 %addtmp
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %calltmp = call i32 @idk(i32 1, i32 2)
  store i32 %calltmp, ptr %x, align 4
  %x1 = load i32, ptr %x, align 4
  %x2 = load i32, ptr %x, align 4
  ret i32 %x2
}

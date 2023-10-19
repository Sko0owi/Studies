let x = x <- WOLNE in x^x <- ZWIĄZANE

let x = 10. in let y = x**2. in y*.x
                       ^        ^  ^
                       Z        Z  Z


let x = 1 and y = x in x + y
                  ^    ^   ^
                  W    Z   Z

let x = 1 in fun y z -> x*y*z
                 ^ ^    ^ ^ ^ 
                 F m    Z W Z
                 
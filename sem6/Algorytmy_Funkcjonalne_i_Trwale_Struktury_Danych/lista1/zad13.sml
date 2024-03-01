datatype tree = & of int * (tree * tree) | %
infix &


val exampleTree = (10&(2&(1&(%,%),3&(25&(%,%),%)),4&(%,%)))


fun member (x,y&(t1,t2)) =
    if x <= y
        then if y <= x
            then true
            else member (x,t1)
        else member (x,t2)
    | member (x,%) = false

fun memberV2 (x, y&(t1,t2)) =
    let 
        fun memberHelper (x, y&(t1,t2), acc) =
        if x <= y 
            then memberHelper (x, t1, y)
            else memberHelper (x, t2, acc)
        | memberHelper (x,%, acc) = acc 
    in 
        if x = memberHelper (x, (y&(t1,t2)), y) 
            then true
            else false
    end


fun insert (x, t as y&(t1,t2)) =
    if x <= y
        then if y <= x
            then t
            else y&(insert (x,t1), t2)
        else y&(t1, insert (x,t2))
    | insert (x,%) = x&(%,%)

fun insertV2 (x, t as y&(t1,t2)) = 
    if memberV2 (x,t) 
        then t 
        else if x <= y
            then y&(insertV2 (x, t1), t2)
            else y&(t1, insertV2(x, t2))
    | insertV2 (x,%) = x&(%,%)


fun inserttop (x, y&(t1,t2), v, org, cps) = 
    if y >= x
            then inserttop (x,t1,y, org, fn e => cps(y&(e,t2)))
            else inserttop (x,t2,v, org, fn e => cps(y&(t1,e)))
    | inserttop (x,%,v,org,cps) = 
        if v = x 
            then org
            else cps(x&(%,%))
package.loadlib("../liblcl.so", "luaopen_containerlib")()

tableDeque = require "deque"

max = 1000000

randoms = {}
for i=1, max do
    table.insert(randoms, math.random(1, 10000))
end

deque = Deque()
tbl_deque = tableDeque()
set = Set()
tbl = {}
tbl_set = {}

local start_clk, end_clk

print("insertions:")

-- add to deque
start_clk = getMicroTime()
for i=1, max do
    deque:push_back(randoms[i])
end
end_clk = getMicroTime()
print("deque push_back time:", end_clk - start_clk)

-- add to table deque
tbl_deque = tableDeque()
start_clk = getMicroTime()
for i=1, max do
    tbl_deque:push_back(randoms[i])
end
end_clk = getMicroTime()
print("table deque push_back time:", end_clk - start_clk)

-- add to table
start_clk = getMicroTime()
for i=1, max do
    table.insert(tbl, randoms[i])
end
end_clk = getMicroTime()
print("table insert time:", end_clk - start_clk)

-- add to set
start_clk = getMicroTime()
for i=1, max do
    set:insert(randoms[i])
end
end_clk = getMicroTime()
print("set insert time:", end_clk - start_clk)

-- add to table set
start_clk = getMicroTime()
for i=1, max do
    local e = tbl_set[randoms[i]]
    if e == nil then
        tbl_set[randoms[i]] = 1
    else
        tbl_set[randoms[i]] = e + 1
    end
end
end_clk = getMicroTime()
print("table set insert time:", end_clk - start_clk)

-- randomized insertion to deque
deque = Deque()
start_clk = getMicroTime()
for i=1, max do
    if (math.random() < 0.5) then
        deque:push_front(randoms[i])
    else
        deque:push_back(randoms[i])
    end
end
end_clk = getMicroTime()
print("deque randomized insert time:", end_clk - start_clk)

-- randomized insertion to table deque
tbl_deque = tableDeque()
start_clk = getMicroTime()
for i=1, max do
    if (math.random() < 0.5) then
        tbl_deque:push_front(randoms[i])
    else
        tbl_deque:push_back(randoms[i])
    end
end
end_clk = getMicroTime()
print("table deque randomized insert time:", end_clk - start_clk)



print("\nrandom access to sets:")

-- lcl set
start_clk = getMicroTime()
for i=1, max do
    assert(set:count(randoms[i]) ~= 0)
end
end_clk = getMicroTime()
print("lcl set random access time:", end_clk - start_clk)

-- table set
start_clk = getMicroTime()
for i=1, max do
    assert(tbl_set[randoms[i]] ~= 0)
end
end_clk = getMicroTime()
print("table set random access time:", end_clk - start_clk)



print("\nsorting:")

-- sort deque
start_clk = getMicroTime()
deque:sort()
end_clk = getMicroTime()
print("deque sort time:", end_clk - start_clk)

-- sort table
start_clk = getMicroTime()
table.sort(tbl)
end_clk = getMicroTime()
print("table sort time:", end_clk - start_clk)

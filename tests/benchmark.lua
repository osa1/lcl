package.loadlib("../liblcl.so", "luaopen_containerlib")()

max = 1000000

randoms = {}
for i=1, max do
    table.insert(randoms, math.random(1, 10000))
end

deque = Deque()
set = Set()
tbl = {}

local start_clk, end_clk

print("insertions:")

-- add to deque
start_clk = os.clock()
for i=1, max do
    deque:push_back(randoms[i])
end
end_clk = os.clock()
print("deque push_back time:", end_clk - start_clk)

-- add to table
start_clk = os.clock()
for i=1, max do
    table.insert(tbl, randoms[i])
end
end_clk = os.clock()
print("table insert time:", end_clk - start_clk)

-- add to set
start_clk = os.clock()
for i=1, max do
    set:insert(randoms[i])
end
end_clk = os.clock()
print("set insert time:", end_clk - start_clk)


print("\nsorting:")

-- sort deque
start_clk = os.clock()
deque:sort()
end_clk = os.clock()
print("deque sort time:", end_clk - start_clk)

-- sort table
start_clk = os.clock()
table.sort(tbl)
end_clk = os.clock()
print("table sort time:", end_clk - start_clk)

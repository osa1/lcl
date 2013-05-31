print("\n---- dynamic load test ----")

print(package.loadlib("/home/omer/cpp/lua-containers/containers.so", "*"))
package.loadlib("/home/omer/cpp/lua-containers/main.so", "luaopen_containerlib")()

Point = {}
Point.__index = Point
Point.__lt = function (p1, p2)
    return p1:dist() < p2:dist()
end

function Point.new(x, y)
    local o = {}
    setmetatable(o, Point)

    o.x = x
    o.y = y

    return o
end

function Point:dist()
    return self.x*self.x + self.y*self.y
end

function Point:__tostring()
    return table.concat({ "Point<", self.x, ", ", self.y, ">" })
end

setmetatable(Point, { __call = function (_, x, y) return Point.new(x, y) end })

set = Set()
set:insert(9)
set:insert(8)
set:insert(7)
print(set)

-- this should behave like min priority queue(min heap)
set = Set()

set:insert(Point(1, 3))
set:insert(Point(2, 3))
set:insert(Point(2, 4))
set:insert(Point(3, 4))
set:insert(Point(1, 4))
set:insert(Point(1, 1))
set:insert(Point(0, 4))

print("set:", set)

while (set:size() ~= 0) do
    print(set:pop():dist())
end

-- this should behave like max priority queue
maxheap = Set(function (p1, p2)
    return p1:dist() > p2:dist()
end)

maxheap:insert(Point(1, 3))
maxheap:insert(Point(2, 3))
maxheap:insert(Point(2, 4))
maxheap:insert(Point(3, 4))
maxheap:insert(Point(1, 4))
maxheap:insert(Point(1, 1))
maxheap:insert(Point(0, 4))

print("maxheap:", maxheap)

while (maxheap:size() ~= 0) do
    print(maxheap:pop():dist())
end

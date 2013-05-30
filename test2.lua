print("\n---- test 2 ----")

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

setmetatable(Point, { __call = function (_, x, y) return Point.new(x, y) end })

-- this should behave like min priority queue(min heap)
set = Set.new()

set:insert(Point(1, 3))
set:insert(Point(2, 3))
set:insert(Point(2, 4))
set:insert(Point(3, 4))
set:insert(Point(1, 4))
set:insert(Point(1, 1))
set:insert(Point(0, 4))

while (set:size() ~= 0) do
    print(set:pop():dist())
end

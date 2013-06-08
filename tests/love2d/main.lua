local set, deque

function love.load()
    love.graphics.setMode(800, 100)
    love.graphics.setBackgroundColor(131, 192, 240, 0)
    package.loadlib("../liblcl.so", "luaopen_containerlib")()
    love.graphics.setFont(love.graphics.newFont(14))

    set = Set()
    deque = Deque()

    for i=0, 1000 do
        set:insert(math.random(0, 100))
        deque:push_back(math.random(0, 100))
        deque:push_front(math.random(0, 100))
    end

    set = tostring(set)
    deque = tostring(deque)
end

function love.draw()
    love.graphics.print(set, 20, 20)
    love.graphics.print(deque, 20, 50)
end

local Deque = {}
Deque.__index = Deque

local Deque_mt = {}
setmetatable(Deque, Deque_mt)

function Deque.new()
    local o = {}
    o.frontIdx = 0
    o.backIdx = 0
    return setmetatable(o, Deque)
end

function Deque_mt.__call(_, ...)
    return Deque.new(...)
end

function Deque:__tostring()
    local t = {}
    table.insert(t, "< deque:")
    for i=self.backIdx+1, self.frontIdx-1, 1 do
        table.insert(t, tostring(self[i]))
    end
    table.insert(t, ">")
    return table.concat(t, " ")
end

function Deque:checkEmpty()
   if self.backIdx+1 > self.frontIdx-1 then
       error "deque is empty"
   end
end

function Deque:push_front(elem)
    self[self.frontIdx] = elem
    self.frontIdx = self.frontIdx + 1
    if self.backIdx == 0 then
        self.backIdx = -1
    end
end

function Deque:push_back(elem)
    self[self.backIdx] = elem
    self.backIdx = self.backIdx - 1
    if self.frontIdx == 0 then
        self.frontIdx = 1
    end
end

function Deque:pop_front()
    self:checkEmpty()
    local ret = self[self.frontIdx-1]
    self[self.frontIdx-1] = nil
    self.frontIdx = self.frontIdx - 1
    return ret
end

function Deque:pop_back()
    self:checkEmpty()
    local ret = self[self.backIdx+1]
    self[self.backIdx+1] = nil
    self.backIdx = self.backIdx + 1
    return ret
end

return Deque

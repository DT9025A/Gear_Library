module('i2c', ...)

SLOW = 0
FAST = 1

function setup(id, speed, slaveaddr)
end

function send(id, slave, data)
end

function recv(id, slave, size)
end

function write(id, reg, data)
end

function read(id, reg, num)
end

function close(id)
end

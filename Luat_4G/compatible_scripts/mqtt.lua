module("mqtt", ...)

function client(clientId, keepAlive, username, password, cleanSession, will, version)
end

function connect(host, port, transport, cert, timeout)
end

function subscribe(topic, qos)
end

function unsubscribe(topic)
end

function publish(topic, payload, qos, retain)
end

function receive(timeout, msg)
end

function disconnect()
end


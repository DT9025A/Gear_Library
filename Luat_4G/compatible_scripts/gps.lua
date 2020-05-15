module("gps", ...)

function open(mode, para)
end

function close(mode, para)
end

function closeAll()
end

function isActive(mode, para)
end

function setPowerCbFnc(cbFnc)
end

function setUart(id, baudrate, databits, parity, stopbits)
end

function setAerialMode(gps, beidou, glonass, galieo)
end

function setNmeaMode(mode, cbFnc)
end

function setRunMode(mode, runTm, sleepTm)
end

function setNemaReportFreq(rmc, gga, gsa, gsv, vtg, gll)
end

function setLocationFilter(seconds)
end

function isOpen()
end

function isFix()
end

function getLocation(typ)
end

function getAltitude()
end

function getSpeed()
end

function getOrgSpeed()
end

function getCourse()
end

function getViewedSateCnt()
end

function getUsedSateCnt()
end

function getGgaloc()
end

function getUtcTime()
end

function getSep()
end

function getSateSn()
end

function getGsv()
end

function setParseItem(utcTime, gsv, gsaId)
end


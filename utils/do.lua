-- Create a file from template
require('os')

local inbuf = io.open('utils/tmpl.cpp', 'r')
local tmpl = inbuf:read("*a")
inbuf:close()

if arg == nil or #arg == 0 then
    arg = {...}
end

if #arg < 1 then
    print("Usage: do.lua [problem identifier]")
    return
end

local fname = 'undone/' .. arg[1] .. '.cpp'

local out = io.open(fname, 'w')
out:write(tmpl)
out:close()

arg = nil

return fname


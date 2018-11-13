require('os');
require('io');
dir = arg[1]
base = arg[2]
name = arg[3]
-- Check if is not at right directory
if not dir:match('undone$') then
    io.stderr:write("Wrong directory: " .. dir .. '\n');
end
-- Remove executable
os.remove(base .. '.obj')
os.remove(base .. 'exe')
-- Get output directory
if base:match('^p%d+$') then
    dest = 'done/'
else
    dest = 'template/'
end
os.rename(dir .. '/' .. name, dir .. '/../' .. dest .. name);

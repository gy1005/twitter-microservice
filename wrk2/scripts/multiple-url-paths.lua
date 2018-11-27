
require "socket"
time = socket.gettime()*1000
math.randomseed(time)
math.random(); math.random(); math.random()

counter = 0


request = function()  
  -- url_path = math.random(131072)
	url_path = math.random(1000)
	-- url_path = math.random(100)
	-- url_path = zipf(1.01, 10000000)
	-- url_path = zipf(1.01, 1000000)
	-- url_path = zipf(1.2, 100000)

    counter = counter + 1
    -- print("req", url_path)
    return wrk.format(nil, "http://localhost:8080/test/" .. tostring(url_path))
    
end

function zipf (s, N)
	p = math.random()
	local tolerance = 0.01
	local x = N / 2;
	
	local D = p * (12 * (math.pow(N, 1 - s) - 1) / (1 - s) + 6 - 6 * math.pow(N, -s) + s - math.pow(N, -1 - s) * s)
	
	while true do 
		local m    = math.pow(x, -2 - s);
		local mx   = m   * x;
		local mxx  = mx  * x;
		local mxxx = mxx * x;

		local a = 12 * (mxxx - 1) / (1 - s) + 6 * (1 - mxx) + (s - (mx * s)) - D
		local b = 12 * mxx + 6 * (s * mx) + (m * s * (s + 1))
		local newx = math.max(1, x - a / b)
		if math.abs(newx - x) <= tolerance then
			return math.floor(newx)
		end
		x = newx
	end
end


response = function(status, headers, body)
	if headers["X-cache"] == "MISS" then
		return 0, headers["request-latency"], headers["get-latency"], headers["find-latency"], headers["set-latency"]
	else
		return 1, headers["request-latency"], headers["get-latency"], 0, 0
	end
end

  

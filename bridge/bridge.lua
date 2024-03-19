local M = {}

---@type function | nil
local reward_callback = nil
---@type function | nil
local error_callback = nil
---@type function | nil
local close_callback
---@type function | nil
local opened_callback

---@type bool
local inited = false

---Initialize the extension before using its functions
function M.init()
    if inited then
        error("bridge already inited")
    end
    if not html5 then
        error("use bridge only in html5 builds")
    end
    inited = true
    bridge.set_callbacks(function (self, callback_id)
        if callback_id == 0 then
            if opened_callback then
                opened_callback(self)
            end
        elseif callback_id == 1 then
            if close_callback then
                close_callback(self)
            end
        elseif callback_id == 2 then
            if reward_callback then
                reward_callback(self)
            end
        elseif callback_id == 3 then
            if error_callback then
                error_callback(self)
            end
        end
    end)
end

function M.get_language()
    return bridge.get_language()
end

---comment
---@param message string
function M.send_platform_message(message)
    bridge.send_platform_message(message)
end

---comment
---@param _reward_callback function | nil
---@param _error_callback function | nil
---@param _close_callback function | nil
---@param _opened_callback function | nil
function M.show_reward_ads(_reward_callback, _error_callback, _close_callback, _opened_callback)
    if not inited then
        error("bridge not inited")
    end

    reward_callback = _reward_callback
    error_callback = _error_callback
    close_callback = _close_callback
    opened_callback = _opened_callback
    bridge.show_reward_ads()
end

---comment
---@param _reward_callback function | nil
---@param _error_callback function | nil
---@param _close_callback function | nil
---@param _opened_callback function | nil
function M.show_interstitial_ads(_reward_callback, _error_callback, _close_callback, _opened_callback)
    if not inited then
        error("bridge not inited")
    end

    reward_callback = _reward_callback
    error_callback = _error_callback
    close_callback = _close_callback
    opened_callback = _opened_callback
    bridge.show_interstitial_ads()
end

return M
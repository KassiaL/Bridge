var BridgeExt = {

    // This can be accessed from the bootstrap code in the .html file
    $lib: {
        no_message_callback: null,

        get_bridge: function(){
            return bridge;
        },

        get_language : function() {
            return bridge.platform.language;
        },
    },

    show_rewarded: function() {
        lib.get_bridge().advertisement.showRewarded();
    },

    show_interstitial: function() {
        lib.get_bridge().advertisement.showInterstitial();
    },

    send_platform_message: function(message) {
        lib.get_bridge().platform.sendMessage(UTF8ToString(message));
    },

    set_callbacks_in_js: function(no_message_callback){
        lib.no_message_callback = no_message_callback;
        lib.get_bridge().advertisement.on(bridge.EVENT_NAME.REWARDED_STATE_CHANGED, state => {
            if (state == "opened") {
                {{{ makeDynCall("vi", "lib.no_message_callback")}}}(0);
            }
            else if (state == "closed") {
                {{{ makeDynCall("vi", "lib.no_message_callback")}}}(1);
            }
            else if (state == "rewarded") {
                {{{ makeDynCall("vi", "lib.no_message_callback")}}}(2);
            }
            else if (state == "failed") {
                {{{ makeDynCall("vi", "lib.no_message_callback")}}}(3);
            }
        });
        lib.get_bridge().advertisement.on(bridge.EVENT_NAME.INTERSTITIAL_STATE_CHANGED, state => {
            if (state == "opened") {
                {{{ makeDynCall("vi", "lib.no_message_callback")}}}(0);
            }
            else if (state == "closed") {
                {{{ makeDynCall("vi", "lib.no_message_callback")}}}(1);
            }
            else if (state == "rewarded") {
                {{{ makeDynCall("vi", "lib.no_message_callback")}}}(2);
            }
            else if (state == "failed") {
                {{{ makeDynCall("vi", "lib.no_message_callback")}}}(3);
            }
        });
    },

    get_language: function() {
        var str = lib.get_language();
        var allocated_array = null;
        if (str != '') {
            allocated_array = _malloc(str.length + 1);         // allocate C++ memory to store it in
            Module.writeStringToMemory(str, allocated_array, false);  // copy the data into the C array
        }
        return allocated_array;
    },
}

autoAddDeps(BridgeExt, '$lib');
mergeInto(LibraryManager.library, BridgeExt);
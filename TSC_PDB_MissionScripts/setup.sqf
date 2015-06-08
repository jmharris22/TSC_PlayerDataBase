//setup.sqf

if (isDedicated) then {
    "packetGear" addPublicVariableEventHandler {
        _pcid = owner (_this select 1);
		//Do work
        _pcid publicVariableClient "packet";
    };
} else {
    "packetGear" addPublicVariableEventHandler {
        _thesum = _this select 1;
        hint str _thesum;
    };
};
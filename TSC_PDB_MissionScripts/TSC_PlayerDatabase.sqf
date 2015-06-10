//TSC_PlayerDatabase.sqf

//Setup

TSC_db_fnc_callBackAddUniform = {
	_pcid = _this select 1;
	packetUniform = _this select 0;
	_pcid publicVariableClient "packetUniform";
};
TSC_db_fnc_callBackAddVest = {
	_pcid = _this select 1;
	packetVest = _this select 0;
	_pcid publicVariableClient "packetVest";
};
TSC_db_fnc_callBackAddHeadgear = {
	_pcid = _this select 1;
	packetHeadgear = _this select 0;
	_pcid publicVariableClient "packetHeadgear";
};
TSC_db_fnc_callBackAddBackpack = {
	_pcid = _this select 1;
	packetBackpack = _this select 0;
	_pcid publicVariableClient "packetBackpack";
};
TSC_db_fnc_callBackAddPrimaryWeapn = {
	_pcid = _this select 1;
	packetPrimaryWeapon = _this select 0;
	_pcid publicVariableClient "packetPrimaryWeapon";
};
TSC_db_fnc_callBackAddSecondaryWeapon = {
	_pcid = _this select 1;
	packetSecondaryWeapon = _this select 0;
	_pcid publicVariableClient "packetSecondaryWeapon";
};
TSC_db_fnc_callBackAddHandgun = {
	_pcid = _this select 1;
	packetHandgun = _this select 0;
	_pcid publicVariableClient "packetHandgun";
};
TSC_db_fnc_callBackSetter = {
	hint _this;
};


KK_fnc_callExtensionAsync = {
    [format ["r:%1","TSC_PlayerDataBase" callExtension format ["s:%1",_this select 0]], _this select 1, _this select 2] spawn {
        waitUntil {
            _res = "TSC_PlayerDataBase" callExtension (_this select 0);
            if (_res != "WAIT") exitWith {
                [_res, _this select 2] call (_this select 1);
                true
            };
            false
        };
    };
};



if (isDedicated) then {
    "packetUniform" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		
		//Do work
		_command = format["s:B%1",_uid];
		_id = [_command, TSC_db_fnc_callBackAddUniform, _pcid] call KK_fnc_callExtensionAsync;
    };
} else {
	
    "packetUniform" addPublicVariableEventHandler {
		hint str _this;
        player forceAddUniform (_this select 1);
    };
};


if (isDedicated) then {
    "packetVest" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
       _command = format["s:C%1",_uid];
	   _id = [_command, TSC_db_fnc_callBackAddVest, _pcid] call KK_fnc_callExtensionAsync;
    };
} else {
    "packetVest" addPublicVariableEventHandler {
        player addVest (_this select 1);
    };
};


if (isDedicated) then {
    "packetHeadgear" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
        _command = format["s:D%1",_uid];
		_id = [_command, TSC_db_fnc_callBackAddHeadgear, _pcid] call KK_fnc_callExtensionAsync;
    };
} else {
    "packetHeadgear" addPublicVariableEventHandler {
		player addHeadgear (_this select 1);
    };
};


if (isDedicated) then {
    "packetPrimaryWeapon" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
        _command = format["s:E%1",_uid];
		_id = [_command, TSC_db_fnc_callBackAddPrimaryWeapn, _pcid] call KK_fnc_callExtensionAsync;
    };
} else {
    "packetPrimaryWeapon" addPublicVariableEventHandler {
        player addWeapon (_this select 1);
    };
};


if (isDedicated) then {
    "packetSecondaryWeapon" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
        _command = format["s:F%1",_uid];
		_id = [_command, TSC_db_fnc_callBackAddSecondaryWeapon, _pcid] call KK_fnc_callExtensionAsync;
    };
} else {
    "packetSecondaryWeapon" addPublicVariableEventHandler {
        player addSecondaryWeaponItem (_this select 1);
    };
};


if (isDedicated) then {
    "packetBackpack" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
        _command = format["s:H%1",_uid];
		_id = [_command, TSC_db_fnc_callBackAddBackpack, _pcid] call KK_fnc_callExtensionAsync;
    };
} else {
    "packetBackpack" addPublicVariableEventHandler {
        player addBackPack (_this select 0);
    };
};


if (isDedicated) then {
    "packetHandgun" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
        _pcid publicVariableClient "packet";
    };
} else {
    "packetHandgun" addPublicVariableEventHandler {
        _thesum = _this select 1;
        hint str _thesum;
    };
};


if (isDedicated) then {
    "packetUniformItems" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
        _pcid publicVariableClient "packet";
    };
} else {
    "packetUniformItems" addPublicVariableEventHandler {
        _thesum = _this select 1;
        hint str _thesum;
    };
};


if (isDedicated) then {
    "packetVestItems" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
        _pcid publicVariableClient "packet";
    };
} else {
    "packetVestItems" addPublicVariableEventHandler {
        _thesum = _this select 1;
        hint str _thesum;
    };
};


if (isDedicated) then {
    "packetBackpackItems" addPublicVariableEventHandler {
        _pcid = owner (_this select 1 select 0);
		_uid = getPlayerUID (_this select 1 select 0);
		//Do work
        _pcid publicVariableClient "packet";
    };
} else {
    "packetBackpackItems" addPublicVariableEventHandler {
        _thesum = _this select 1;
        hint str _thesum;
    };
};





//Player init
if(!isDedicated) then {
	
	waitUntil {!isNull player};
	packetUniform = [player];
	publicVariableServer "packetUniform";
	
	packetVest = [player];
	publicVariableServer "packetVest";
	
	packetHeadgear = [player];
	publicVariableServer "packetHeadgear";
	
	packetBackpack = [player];
	publicVariableServer "packetBackpack";
	
	packetPrimaryWeapon = [player];
	publicVariableServer "packetPrimaryWeapon";
	
	packetSecondaryWeapon = [player];
	publicVariableServer "packetSecondaryWeapon";
}
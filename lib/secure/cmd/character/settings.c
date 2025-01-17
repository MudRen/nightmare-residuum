inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("settings ([setting] [value])");
    set_help_text("The settings command is used to view your account's settings.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc, user;
    string *list = ({ });
    mapping header;

    if (input && tc->query_immortal()) {
        if (find_character(input)) {
            target = find_character(input);
            input = 0;
        }
    }

    user = target->query_user();

    if (input) {
        string key, *split;
        mixed value;

        split = explode(input, " ");
        key = split[0];
        value = implode(split[1..], " ");

        switch (key) {
            case "width": case "lines":
                value = to_int(value);
                break;
        }
        header = ([
            "columns": 1,
            "align": "center",
        ]);
        if (member_array(key, keys(user->query_settings())) == -1) {
            header["items"] = ({ "Invalid setting: " + key });
        } else {
            user->set_setting(key, value);
            header["items"] = ({ "Setting %^BOLD%^" + key + "%^BOLD_OFF%^ to %^BOLD%^" + value + "%^BOLD_OFF%^" });
        }
    }

    foreach (string key,mixed value in user->query_settings()) {
        list += ({ sprintf("%16s : %s", key, ""+value) });
    }

    border(([
        "title": "SETTINGS",
        "subtitle": target->query_cap_name(),
        "header": header,
        "body": ([
            "items": list,
            "columns": 1,
        ]),
    ]));
}
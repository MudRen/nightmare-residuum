private string query_account_setting (string setting) {
    object po = previous_object(-1)[<1], account;

    if (strsrch(D_TEST, base_name(po)) == 0) po = previous_object(-1)[<2];

    if (!(account = po->query_account())) return 0;

    return account->query_setting(setting);
}

varargs string format_page (string *items, int columns, int pad, int center) {
    int width, w, i, j, n, r;
    string *rows = ({});

    if (!arrayp(items) || !sizeof(items)) error("Bad argument 1 to format->format_page");

    if (!columns) columns = 2;
    width = to_int(query_account_setting("width")) - pad * 2;
    w = width / columns; // width of column
    n = sizeof(items); // number of columns
    r = width - (w * columns); // remainder
    items = map(items, (: "" + $1 :));

    for (i = 0; i < n; i += columns) {
        string row = "";
        for (j = 0; j < columns; j ++) {
            string tmp;
            if (i + j >= n) { // ran out of columns to fill final row
                row += sprintf("%' '"+sprintf("%d", w*(columns-j))+"s", " ");
                break;
            }
            // check text length without ANSI color
            if (sizeof(tmp = SEFUN->strip_colour(items[i + j])) > w) {
                // use stripped text when its longer than w
                row += tmp[0..w-1];
            } else {
                // account for any color codes in the text
                int diff = sizeof(items[i+j]) - sizeof(tmp);

                if (center) {
                    row += sprintf("%|"+sprintf("%d", w+diff)+"s", ""+items[i + j]);
                } else {
                    row += sprintf("%-"+sprintf("%d", w+diff)+"s", ""+items[i + j]);
                }

                // when color codes present and we're not displaying a RESET, add one
                if (diff > 0 && items[i + j][<9..] != "%^RESET%^") row += "%^RESET%^";
            }
        }
        if (r) row += sprintf("%' '"+sprintf("%d", r)+"s", "");
        rows += ({ row });
    }
    return implode(rows, "\n");
}

string format_exit_brief (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_brief");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "north": result += ({ "n" }); break;
            case "northeast": result += ({ "ne" }); break;
            case "east": result += ({ "e" }); break;
            case "southeast": result += ({ "se" }); break;
            case "south": result += ({ "s" }); break;
            case "southwest": result += ({ "sw" }); break;
            case "west": result += ({ "w" }); break;
            case "northwest": result += ({ "nw" }); break;
            case "up": result += ({ "u" }); break;
            case "down": result += ({ "d" }); break;
            case "enter": result += ({ "ent" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}
string format_exit_verbose (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_verbose");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "n": result += ({ "north" }); break;
            case "ne": result += ({ "northeast" }); break;
            case "e": result += ({ "east" }); break;
            case "se": result += ({ "southeast" }); break;
            case "s": result += ({ "south" }); break;
            case "sw": result += ({ "southwest" }); break;
            case "w": result += ({ "west" }); break;
            case "nw": result += ({ "northwest" }); break;
            case "u": result += ({ "up" }); break;
            case "d": result += ({ "down" }); break;
            case "ent": result += ({ "enter" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}
string format_exit_reverse (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_reverse");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "north": result += ({ "south" }); break;
            case "northeast": result += ({ "southwest" }); break;
            case "east": result += ({ "west" }); break;
            case "southeast": result += ({ "northwest" }); break;
            case "south": result += ({ "north" }); break;
            case "southwest": result += ({ "northeast" }); break;
            case "west": result += ({ "east" }); break;
            case "northwest": result += ({ "southeast" }); break;
            case "up": result += ({ "down" }); break;
            case "down": result += ({ "up" }); break;
            case "enter": result += ({ "out" }); break;
            case "out": result += ({ "enter" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}

string format_stat_brief (string stat) {
    string result = "";
    if (!stringp(stat)) error("Bad argument 1 to format->format_stat_brief");
    switch (stat) {
        case "strength": case "str": result = "str"; break;
        case "perception": case "per": result = "per"; break;
        case "endurance": case "end": result = "end"; break;
        case "charisma": case "cha": result = "cha"; break;
        case "intelligence": case "int": result = "int"; break;
        case "agility": case "agi": result = "agi"; break;
        case "luck": case "lck": result = "lck"; break;
    }
    return result;
}
string format_stat_verbose (string stat) {
    string result = "";
    if (!stringp(stat)) error("Bad argument 1 to format->format_stat_verbose");
    switch (stat) {
        case "str": case "strength": result = "strength"; break;
        case "per": case "perception": result = "perception"; break;
        case "end": case "endurance": result = "endurance"; break;
        case "cha": case "charisma": result = "charisma"; break;
        case "int": case "intelligence": result = "intelligence"; break;
        case "agi": case "agility": result = "agility"; break;
        case "lck": case "luck": result = "luck"; break;
    }
    return result;
}

string format_integer (int num) {
    string *digits, result = "";
    int neg, s;

    if (!intp(num)) error("Bad argument 1 to format->format_integer");

    neg = (num < 0);
    num = abs(num);
    digits = explode(""+to_int(num), "");
    s = sizeof(digits) - 1;

    for(int i = s; i >= 0; i --) {
        result = digits[i] + (i != s && (s-i)%3 == 0 ? "," : "") + result;
    }

    return (neg ? "-" : "") + result;
}


// create a unicode border around action output
// https://en.wikipedia.org/wiki/Box-drawing_character#Box_Drawing
//
// mapping data = ([
// /*
//     "title": "TITLE",
//     "subtitle": "Subtitle",
//     "header": ([
//         "items": ({}),
//         "columns": 2,
//         "align": "left|center",
//     ]),
//     "body": ([
//         "header": "string" || ({ /* array of items */ }),
//         "items": ({}),
//         "columns": 2,
//         "align": "left|center",
//     ]) || ({ /* array of mappings */ }),
//     "footer": ([
//         "items": ({}),
//         "columns": 2,
//         "align": "left|center",
//     ]),
// */
// ]);
private nosave mapping __Border = ([
    "utf-8": ([
        // lines
        "h": "─",
        "v": "│",
        // corners
        "tl": "┌",
        "tr": "┐",
        "br": "┘",
        "bl": "└",
        // joints
        "t": "┬",
        "b": "┴",
        "l": "├",
        "r": "┤",
    ]),
    "US-ASCII": ([
        // lines
        "h": "-",
        "v": "|",
        // corners
        "tl": ".",
        "tr": ".",
        "br": "'",
        "bl": "'",
        // joints
        "t": "-",
        "b": "-",
        "l": "|",
        "r": "|",
    ]),
]);

string *format_border (mapping data) {
    string *lines = ({ }), line;

    int fTitle = !undefinedp(data["title"]);
    int fSubtitle = !undefinedp(data["subtitle"]);
    int fHeader = !undefinedp(data["header"]);
    int fBody = !undefinedp(data["body"]);
    int fFooter = !undefinedp(data["footer"]);

    if (query_account_setting("screenreader") != "on") {
        mapping b = __Border[query_encoding()];
        int width, ansi, n;

        ansi = query_account_setting("ansi") == "on";
        width = to_int(query_account_setting("width"));
        n = 0;

        if (fTitle) {
            // Title Line 1
            line = (ansi?"%^RESET%^CYAN%^":"") + "   " + b["tl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", 2 + strlen(data["title"]) + (fSubtitle ? 2 + strlen(data["subtitle"]) : 0))+"s", "") + b["tr"];
            lines += ({ line });

            // Title Line 2
            line = b["tl"] + (fHeader ? b["t"] : b["h"]) + b["h"];
            line += b["r"] + " " + (ansi?"%^WHITE%^BOLD%^":"") + data["title"];
            n += 5 + strlen(data["title"]);
            if (fSubtitle) {
                line += ":" + (ansi?"%^BOLD_OFF%^":"") + " " + data["subtitle"];
                n += 2 + strlen(data["subtitle"]);
            } else {
                line += (ansi?"%^BOLD_OFF%^":"");
            }
            line += " " + (ansi?"%^CYAN%^":"") + b["l"] + b["h"];
            n += 3;
            line += sprintf("%'"+b["h"]+"'"+sprintf("%d", width-2-n)+"s", "");
            line += (fHeader ? b["t"] : b["h"]) + b["tr"];
            lines += ({ line });

            // Title Line 3
            line = b["v"] + (fHeader ? b["v"] : " ") + " ";
            line += b["bl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", 2 + strlen(data["title"]) + (data["subtitle"] ? 2 + strlen(data["subtitle"]) : 0))+"s", "") + b["br"];
            line += sprintf("%' '"+sprintf("%d", width-1-n)+"s", "");
            line += (fHeader ? b["v"] : " ") + b["v"];
            lines += ({ line });

        } else { // no title
            line = (ansi?"%^RESET%^CYAN%^":"") + b["tl"] + (fHeader ? b["t"] : b["h"]) + sprintf("%'"+b["h"]+"'"+sprintf("%d", width-4)+"s", "") + (fHeader ? b["t"] : b["h"]) + b["tr"];
            lines += ({ line });
        }

        if (fHeader) {
            string format;
            int center = (data["header"]["align"] == "center");
            // Header header
            if (stringp(data["header"]["header"])) {
                line = b["v"] + b["v"] + "  " + (ansi?"%^WHITE%^BOLD%^":"") + data["header"]["header"] + (ansi?"%^BOLD_OFF%^CYAN%^":"") + sprintf("%' '"+sprintf("%d", width - 8 - strlen(data["header"]["header"]))+"s", "") + "  " + b["v"] + b["v"];
                lines += ({ line });
            } else if (arrayp(data["header"]["header"])) {
                line = b["v"] + b["v"] + "  " + (ansi?"%^WHITE%^BOLD%^":"") + format_page(data["header"]["header"], data["header"]["columns"], 4, center) + (ansi?"%^BOLD_OFF%^CYAN%^":"") + "  " + b["v"] + b["v"];
                lines += ({ line });
            }
            // Header lines
            format = sizeof(data["header"]["items"]) > 0 ? format_page(data["header"]["items"], data["header"]["columns"], 4, center) : "";
            foreach (string l in explode(format, "\n")) {
                line = b["v"] + b["v"] + "  " + (ansi ? "%^RESET%^" + l + "%^CYAN%^" : l) + "  " + b["v"] + b["v"];
                lines += ({ line });
            }
            // Header bottom line
            line = b["v"] + b["bl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", width - 4)+"s", "") + b["br"] + b["v"];
            lines += ({ line });
        }

        if (fBody && mapp(data["body"])) {
            data["body"] = ({ data["body"] });
        }
        if (fBody && arrayp(data["body"])) {
            // Body top line
            line = b["v"] + sprintf("%' '"+sprintf("%d", width - 2)+"s", "") + b["v"];
            lines += ({ line });
            foreach (mapping child in data["body"]) {
                string format;
                int center = (child["align"] == "center");
                // Body child header
                if (stringp(child["header"])) {
                    line = b["v"] + "   " + (ansi?"%^WHITE%^BOLD%^":"") + child["header"] + (ansi?"%^BOLD_OFF%^CYAN%^":"") + sprintf("%' '"+sprintf("%d", width - 8 - strlen(child["header"]))+"s", "") + "   " + b["v"];
                } else if (arrayp(child["header"])) {
                    line = b["v"] + "   " + (ansi?"%^WHITE%^BOLD%^":"") + format_page(child["header"], child["columns"], 4, center) + (ansi?"%^BOLD_OFF%^CYAN%^":"") + "   " + b["v"];
                }
                lines += ({ line });
                // Body child lines
                format = sizeof(child["items"]) > 0 ? format_page(child["items"], child["columns"], 4, center) : "";
                foreach (string l in explode(format, "\n")) {
                    line = b["v"] + "   " + (ansi ? "%^RESET%^" + l + "%^CYAN%^" : l) + "   " + b["v"];
                    lines += ({ line });
                }
                // Body child bottom line
                line = b["v"] + sprintf("%' '"+sprintf("%d", width - 2)+"s", "") + b["v"];
                lines += ({ line });
            }
        }

        if (fFooter) {
            string format;
            int center = (data["footer"]["align"] == "center");
            // Footer top line
            line = b["v"] + b["tl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", width - 4)+"s", "") + b["tr"] + b["v"];
            lines += ({ line });
            // Header header
            if (stringp(data["footer"]["header"])) {
                line = b["v"] + b["v"] + "  " + (ansi?"%^WHITE%^BOLD%^":"") + data["footer"]["header"] + (ansi?"%^BOLD_OFF%^CYAN%^":"") + sprintf("%' '"+sprintf("%d", width - 8 - strlen(data["footer"]["header"]))+"s", "") + "  " + b["v"] + b["v"];
                lines += ({ line });
            } else if (arrayp(data["footer"]["header"])) {
                line = b["v"] + b["v"] + "  " + (ansi?"%^WHITE%^BOLD%^":"") + format_page(data["footer"]["header"], data["footer"]["columns"], 4, center) + (ansi?"%^BOLD_OFF%^CYAN%^":"") + "  " + b["v"] + b["v"];
                lines += ({ line });
            }
            // Footer lines
            format = sizeof(data["footer"]["items"]) > 0 ? format_page(data["footer"]["items"], data["footer"]["columns"], 4, center) : "";
            foreach (string l in explode(format, "\n")) {
                line = b["v"] + b["v"] + "  " + (ansi ? "%^RESET%^" + l + "%^CYAN%^" : l) + "  " + b["v"] + b["v"];
                lines += ({ line });
            }
            // Border Bottom line
            line = b["bl"] + b["b"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", width - 4)+"s", "") + b["b"] + b["br"] + (ansi?"%^RESET%^":"");
            lines += ({ line });
        } else {
            // Border Bottom line
            line = b["bl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", width - 2)+"s", "") + b["br"] + (ansi?"%^RESET%^":"");
            lines += ({ line });
        }
    } else { // screenreader
        if (fTitle) {
            line = data["title"] + (fSubtitle ? ": " + data["subtitle"] : "");
            lines += ({ line });
        }
        if (fHeader) {
            foreach (string item in data["header"]["items"]) {
                lines += ({ item });
            }
        }
        if (fBody) {
            foreach (string item in data["body"]["items"]) {
                lines += ({ item });
            }
        }
        if (fFooter) {
            foreach (string item in data["footer"]["items"]) {
                lines += ({ item });
            }
        }
    }

    return lines;
}
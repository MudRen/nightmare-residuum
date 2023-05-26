string identify (mixed args...) {
    mixed a;
    int i, l;
    string ret;
    mapping RealMap;

    if (sizeof(args)) {
        a = args[0];
    }

    if (undefinedp(a)) {
        return "UNDEFINED";
    }
    if (intp(a) || floatp(a)) {
        return "" + a;
    }
    if (objectp(a)) {
        if (ret = a->query_key_name()) ret += " ";
        else ret = "";
        return "OBJ(" + ret + file_name(a) + ")";
    }
    if (stringp(a)) {
        a = replace_string(a, "\"", "\\\"");
        a = "\"" + a + "\"";
        a = replace_string(a, "\\", "\\\\");
        a = replace_string(a, "\\\"", "\"");
        a = replace_string(a, "\n", "\\n");
        a = replace_string(a, "\t", "\\t");
        return a;
    }
    if (mapp(a)) {
        ret = "([ ";
        RealMap = a;
        a = sort_array(keys(RealMap), 1);
        l = sizeof(a);
        for (i = 0 ; i < l ; i ++) {
            if (i) ret += ", ";
            ret += identify(a[i]) + ": " + identify(RealMap[a[i]]);
        }
        return ret + (l ? " " : "") + "])";
    }
    if (functionp(a)) {
        return sprintf("%O", a);
    }
    if (classp(a)) {
        return replace_string(sprintf("%O", a), "\n", "");
    }
    if (arrayp(a)) {
        ret = "({ ";
        l = sizeof(a);
        for (i = 0; i < l; i ++) {
            if (i) ret += ", ";
            ret += identify(a[i]);
        }
        return ret + (l ? " " : "") + "})";
    }
    if (bufferp(a)) {
        l = sizeof(a);
        ret = "BUFFER (";
        for (i = 0; i < l; i ++) {
            ret += "0x" + sprintf("%X", a[i]);
            if (i < l-1) ret += ", ";
        }
        ret += ")";
        return ret;
    }
    // UNKNOWN should never be returned
    return "UNKNOWN";
}

varargs string wrap (string str, int width, int indent, int rawANSI) {
    mapping term;
    object po = previous_object();

    if (!str || !stringp(str)) return "";
    if (undefinedp(width) || width < 0) {
        if (!(width = po->query_setting("width"))) {
            width = 80;
        }
    }

    if (rawANSI) {
        term = ([ ]);
    } else if (po && po->query_setting("ansi") == "on") {
        term = D_ANSI->query_ansi_term();
    } else {
        term = D_ANSI->query_unknown_term();
    }
    str = terminal_colour(str, term, width, indent);
    // strip default ANSI reset color tag added by fluffos
    str = replace_string(str, "\e[49;49m", "");
    str = replace_string(str, "\e[0;10m", "");
    if (strlen(str) > __LARGEST_PRINTABLE_STRING__) {
        str = str[0..__LARGEST_PRINTABLE_STRING__-1];
    }
    return str;
}

int string_compare_same_until (string a, string b) {
    // Returns n where a[0..n] == b[0..n]
    int n = 0, l;

    if ((l = strlen(a)) == strlen(b) && a == b) return l;
    if (strlen(a) > strlen(b)) l = strlen(b);

    for (int i = 0; i < l; i ++) {
        if (a[i] != b[i]) break;
        n ++;
    }
    return n;
}

int string_all_same_character (string str) {
    mapping m = ([ ]);
    foreach (string s in explode(str, "") || ({ })) {
        m[s] = 1;
    }
    return sizeof(m) == 1;
}

string sanitize_name (string name) {
    if (undefinedp(name) || !stringp(name)) error("Bad argument 1 to string->sanitize_name");
    name = replace_string(name, " ", "");
    name = replace_string(name, "'", "");
    name = replace_string(name, "-", "");
    return lower_case(name);
}
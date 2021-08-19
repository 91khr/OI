import gdb
import os

class Check_ConsPrinter:
    """Pretty print a Cons object in check.cpp"""

    def __init__(self, val):
        self.val = val

    def to_string(self):
        def catcons(val):
            if len(val.type.fields()) > 1:
                return [str(val["car"])] + catcons(val["cdr"])
            else:
                return []
        res = catcons(self.val)
        return str(self.val.type) + ("{}" if res == [] else "{{ {} }}".format(", ".join(res)))

def build_pretty_printers():
    p = gdb.printing.RegexpCollectionPrettyPrinter("Senioria_Debug")
    p.add_printer('Cons', '^Cons<.*>$', Check_ConsPrinter)
    return p

gdb.printing.register_pretty_printer(
        gdb.current_progspace(),
        build_pretty_printers())

class Multiprint(gdb.Command):
    """Print multiple values at a time"""

    def __init__(self):
        super(Multiprint, self).__init__("mp", gdb.COMMAND_DATA, gdb.COMPLETE_EXPRESSION)

    def invoke(self, arg, from_tty):
        del self, from_tty  # Unused
        for arg in gdb.string_to_argv(arg):
            gdb.execute(f"p {arg}", False)

Multiprint()  # Init the command


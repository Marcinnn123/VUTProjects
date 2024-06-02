# VUT FIT 2024 - IPP
# Marcin Sochacki
# xsocha03
import sys
import xml.etree.ElementTree as ET
import re
import xml.dom.minidom
import xml.sax.saxutils
import select

SYN_OR_LEX_ERROR = 23  #Argument's syntax or lexical error
HEADER_ERROR = 21  #Header not found
COMMAND_LINE_ERROR = 10  #Too many parameters
OPCODE_ERROR = 22  #Unknown OPCODE

##Handles command line arguments.
#
#Prints help information if '--help' option is provided.
#
def command_line():
    if len(sys.argv) > 2:
        sys.exit(COMMAND_LINE_ERROR)
    elif len(sys.argv) == 2:
        if sys.argv[1] == "--help":
            print("Help: Skript typu filtr (parse.py v jazyce Python 3.10)")
            print("nacte ze standardniho vstupu zdrojovy kod v IPPcode24,")
            print("zkontroluje lexikalni a syntaktickou spravnost kodu")
            print("a vypise na standardni vystup XML reprezentaci programu.")
            sys.exit(0)
        else:
            sys.exit(COMMAND_LINE_ERROR)

##Parses arguments and constructs XML elements based on types.
#
#@param argument: The argument to parse.
#@param number_of_arg: The argument's number.
#@param arg_types: Type of the argument (var, symb,type, label).
#@param xml_pointer: Pointer to the XML element to add the parsed argument.
#
def argumets_parse(argument, number_of_arg, arg_types, xml_pointer):
    var_regex = r'^(GF|LF|TF)@([a-zA-Z_\$\-\&\;\%\*\!\?][0-9]*)+$'
    int_regex = r'^int@[-+]?(0[xX][0-9a-fA-F]+|\d+|0[oO][0-7]+)$'
    bool_regex = r'^(bool)@(true|false)$'
    string_regex = r'^(string)@([a-žA-Z0-9\$\-\&\;\/\%\*\+\§\.\!\?\@\,\§\<\>\=\(\)]?([\\\\]?[0-9]{3})*)+$'
    label_regex = r'^(?!int@|bool@|string@|nil@|GF@|LF@|TF@)[a-zA-Z0-9_\$\-\&\;\%\*\!\?]+$'
    null_regex = r'^nil@nil$'
    type_regex = r'^(int|bool|string)$'

    #Parse the argument based on its typ
    if arg_types == 1: #<var>
        if re.match(var_regex, argument):
            element = ET.SubElement(xml_pointer, 'arg' + str(number_of_arg))
            element.set('type', 'var')
            element.text = argument
        else:
            sys.exit(SYN_OR_LEX_ERROR)
    elif arg_types == 2: #<symb>
        if re.match(var_regex, argument):
            element = ET.SubElement(xml_pointer, 'arg' + str(number_of_arg))
            element.set('type', 'var')
            element.text = argument
        elif re.match(int_regex, argument):
            param = argument.split('@')
            element = ET.SubElement(xml_pointer, 'arg' + str(number_of_arg))
            element.set('type', 'int')
            element.text = param[1]
        elif re.match(bool_regex, argument):
            param = argument.split('@')
            element = ET.SubElement(xml_pointer, 'arg' + str(number_of_arg))
            element.set('type', 'bool')
            element.text = param[1]
        elif re.match(string_regex, argument):
            param = argument.split('@', 1)
            element = ET.SubElement(xml_pointer, 'arg' + str(number_of_arg))
            element.set('type', 'string')
            escaped_string = xml.sax.saxutils.unescape(param[1])
            element.text = escaped_string
        elif re.match(null_regex, argument):
            param = argument.split('@')
            element = ET.SubElement(xml_pointer, 'arg' + str(number_of_arg))
            element.set('type', 'nil')
            element.text = param[1]
        else:
            sys.exit(SYN_OR_LEX_ERROR)
    elif arg_types == 3: #<type>
        if re.match(type_regex, argument):
            element = ET.SubElement(xml_pointer, 'arg' + str(number_of_arg))
            element.set('type', 'type')
            element.text = argument
        else:
            sys.exit(SYN_OR_LEX_ERROR)
    elif arg_types == 4: #<label>
        if re.match(label_regex, argument):
            element = ET.SubElement(xml_pointer, 'arg' + str(number_of_arg))
            element.set('type', 'label')
            element.text = argument
        else:
            sys.exit(SYN_OR_LEX_ERROR)
    else:
        sys.exit(SYN_OR_LEX_ERROR)

##Parses opcodes and constructs XML instructions.
#
#@param order: The order of the instruction.
#@param opcode: The opcode of the instruction.
#@param xml_pointer: The XML pointer to which the instruction will be added.
#@return: The created instruction element.
#
def opcode_parse(order, opcode, xml_pointer):
    #Get the root of memory
    instruction = ET.SubElement(xml_pointer.getroot(), 'instruction')
    instruction.set('order', str(order))
    instruction.set('opcode', opcode)
    return instruction  #Return the created instruction element

#This function creates an ElementTree object and constructs the root
#element for the XML document representing the program header.
#
#@return: The constructed ElementTree object.
#
def header():
    xw = ET.ElementTree()  #Create an ElementTree object
    root = ET.Element('program')
    root.set('language', 'IPPcode24')
    xw._setroot(root)  #Set the root element of the tree

    #Read the first line of input
    if sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
        data = sys.stdin.readline()
    else:
        #No input
        sys.exit(HEADER_ERROR)

    if not data:
        sys.exit(HEADER_ERROR)

    while True:
        #Check for IPPCODE24
        if data.lstrip().upper().startswith('.IPPCODE24'):
            new_string = data.lstrip().upper().replace(".IPPCODE24", "")
            if new_string.strip().startswith('#') or new_string.strip() == '':
                break
            else:
                sys.exit(HEADER_ERROR)
        #Skip empty lines and comments
        elif (data.strip() == '' and len(data) > 0) or data.startswith('#'):
            data = sys.stdin.readline()
        elif not data:
            sys.exit(HEADER_ERROR)
        else:
            sys.exit(HEADER_ERROR)

    return xw

##This function parses the main program instructions and constructs XML instructions
#based on the provided opcode and its arguments.
#
#@param xw: The ElementTree object to which instructions will be added.
#@param counter: The current instruction counter.
#@return: The updated ElementTree object.
#
def main_parse(xw, counter):
    while True:
        comment = False

        data = sys.stdin.readline()
        
        #Skip empty lines or lines with only whitespace
        if data == '\n' or (len(data.strip()) == 0 and len(data) > 0):   
            continue
        
        if not data:
            break 
            
        splited_line = data.strip().split() #Split the arguments and opcode
        comment_line = list(splited_line[0])

        #Skip comment lines
        if comment_line[0] == '#':
            continue
        
        skip = False
        type_of_argument = 1
        splited_line[0] = splited_line[0].upper()
        opcode_value = splited_line[0]

        #Handle different opcodes
        if opcode_value in ['CALL', 'JUMP', 'LABEL']:
            type_of_argument += 3
            counter += 1
            instruction = opcode_parse(counter, splited_line[0], xw)
            if len(splited_line) < 2:
                sys.exit(SYN_OR_LEX_ERROR)

            argument = splited_line[1].split('#')

            argumets_parse(argument[0], 1, type_of_argument, instruction)

            #Checking if there is a start of a comment in the argument
            if '#' not in splited_line[1]:
                if len(splited_line) > 2 and splited_line[2]:
                    comment = list(splited_line[2])
                    if comment[0] != '#':
                        #found extra argument
                        sys.exit(SYN_OR_LEX_ERROR)

        elif opcode_value in ['EXIT', 'PUSHS', 'DPRINT', 'WRITE']:
            type_of_argument += 1
            counter += 1
            instruction = opcode_parse(counter, splited_line[0], xw)
            if len(splited_line) < 2:
                sys.exit(SYN_OR_LEX_ERROR)

            argument = splited_line[1].split('#')

            argumets_parse(argument[0], 1, type_of_argument, instruction)

            #Checking if there is a start of a comment in the argument
            if '#' not in splited_line[1]:
                if len(splited_line) > 2 and splited_line[2]:
                    comment = list(splited_line[2])
                    if comment[0] != '#':
                        #found extra argument
                        sys.exit(SYN_OR_LEX_ERROR)

        elif opcode_value in ['DEFVAR', 'POPS']:
            counter += 1
            instruction = opcode_parse(counter, splited_line[0], xw)
            
            if len(splited_line) < 2:
                sys.exit(SYN_OR_LEX_ERROR)
                
            argument = splited_line[1].split('#')

            argumets_parse(argument[0], 1, type_of_argument, instruction)

            #Checking if there is a start of a comment in the argument
            if '#' not in splited_line[1]:
                if len(splited_line) > 2 and splited_line[2]:
                    comment = list(splited_line[2])
                    if comment[0] != '#':
                        #found extra argument
                        sys.exit(SYN_OR_LEX_ERROR)

        elif opcode_value in ['RETURN', 'BREAK', 'CREATEFRAME', 'PUSHFRAME', 'POPFRAME', 'RETURN#', 'BREAK#', 'CREATEFRAME#', 'PUSHFRAME#', 'POPFRAME#']: 
            counter += 1
            splited_line[0] = splited_line[0].split('#')[0]
            
            instruction = opcode_parse(counter, splited_line[0], xw)

            #Check if there are enough tokens in the line
            if len(splited_line) > 1:
                comment = list(splited_line[1])
                if comment[0] != '#' and '#' not in splited_line[0]:
                    sys.exit(SYN_OR_LEX_ERROR)

        elif opcode_value in ['JUMPIFEQ', 'JUMPIFNEQ']:
            type_of_argument += 3
            skip = True
            counter += 1
            
            #Check if there are enough tokens in the line
            if len(splited_line) < 3:
                sys.exit(SYN_OR_LEX_ERROR)

            #Create an instruction with the parsed opcode
            instruction = opcode_parse(counter, splited_line[0], xw)

            #Parse and add the first argument
            argumets_parse(splited_line[1], 1, type_of_argument, instruction)

            #Check if skip is True and type_of_argument is not 4
            if skip == True and type_of_argument != 4:
                argument = splited_line[2].split('#')

                #Parse and add the second argument
                argumets_parse(argument[0], 2, 2, instruction)

                #Check for comments after the second argument
                if '#' not in splited_line[2]:
                    if len(splited_line) >= 4 and splited_line[3] != '':
                        comment = list(splited_line[3])
                        if comment[0] != '#':
                            sys.exit(SYN_OR_LEX_ERROR)

            #If skip is False or type_of_argument is 4
            if skip == False or type_of_argument == 4:
                #Parse and add the second argument
                argumets_parse(splited_line[2], 2, 2, instruction)

                #Check for the minimum length of tokens
                if len(splited_line) < 4:
                    sys.exit(SYN_OR_LEX_ERROR)

                #Check for comments after the fourth argument
                if len(splited_line) >= 5 and splited_line[4] != '':
                    comment = list(splited_line[4])
                    if comment[0] != '#':
                        sys.exit(SYN_OR_LEX_ERROR)

                #Parse and add the third argument
                argument = splited_line[3].split('#')

                argumets_parse(argument[0], 3, 2, instruction)


        elif opcode_value in ['TYPE', 'INT2CHAR', 'STRLEN', 'MOVE', 'NOT']:
            skip = True

            counter += 1
            if len(splited_line) < 3:
                sys.exit(SYN_OR_LEX_ERROR)

            instruction = opcode_parse(counter, splited_line[0], xw)

            argumets_parse(splited_line[1], 1, type_of_argument, instruction)

            if skip == True and type_of_argument != 4:
                argument = splited_line[2].split('#')

                argumets_parse(argument[0], 2, 2, instruction)

                if '#' not in splited_line[2]:
                    if len(splited_line) >= 4 and splited_line[3] != '':
                        comment = list(splited_line[3])
                        if comment[0] != '#':
                            sys.exit(SYN_OR_LEX_ERROR)

            if skip == False or type_of_argument == 4:
                argumets_parse(splited_line[2], 2, 2, instruction)

                if len(splited_line) < 4:
                    sys.exit(SYN_OR_LEX_ERROR)

                if len(splited_line) >= 4 and splited_line[3] != '':
                    comment = list(splited_line[3])
                    if comment[0] != '#':
                        sys.exit(SYN_OR_LEX_ERROR)

                argument = splited_line[3].split('#')

                argumets_parse(argument[0], 3, 2, instruction)

        elif opcode_value in ['CONCAT', 'STRI2INT', 'ADD', 'SUB', 'MUL', 'IDIV', 'SETCHAR', 'GETCHAR', 'LT', 'GT', 'EQ', 'AND', 'OR']: 
            counter += 1
            
            if len(splited_line) < 3:
                sys.exit(SYN_OR_LEX_ERROR)

            instruction = opcode_parse(counter, splited_line[0], xw)

            argumets_parse(splited_line[1], 1, type_of_argument, instruction)

            if skip == True and type_of_argument != 4:
                argument = splited_line[2].split('#')

                argumets_parse(argument[0], 2, 2, instruction)

                if '#' not in splited_line[2]:
                    if len(splited_line) >= 4 and splited_line[3] != '':
                        comment = list(splited_line[3])
                        if comment[0] != '#':
                            sys.exit(SYN_OR_LEX_ERROR)

            if skip == False or type_of_argument == 4:
                argumets_parse(splited_line[2], 2, 2, instruction)

                if len(splited_line) < 4:
                    sys.exit(SYN_OR_LEX_ERROR)

                if len(splited_line) >= 5 and splited_line[4] != '':
                    comment = list(splited_line[4])
                    if comment[0] != '#':
                        sys.exit(SYN_OR_LEX_ERROR)

                argument = splited_line[3].split('#')

                argumets_parse(argument[0], 3, 2, instruction)

        elif opcode_value in ['READ']: 
            counter += 1
            
            #Check if there are enough tokens in the line
            if len(splited_line) < 3:
                sys.exit(SYN_OR_LEX_ERROR)
            #Create an instruction with the parsed opcode
            instruction = opcode_parse(counter, splited_line[0], xw)
            #Parse and add the first argument
            argumets_parse(splited_line[1], 1, 1, instruction)
            #Check if the second argument is missing
            if len(splited_line) >= 3 and splited_line[2] == '':
                sys.exit(SYN_OR_LEX_ERROR)
            #Check for comments after the second argument
            if len(splited_line) >= 4 and splited_line[3] != '':
                comment = list(splited_line[3])
                if comment[0] != '#':
                    sys.exit(SYN_OR_LEX_ERROR)
            #Parse and add the second argument
            argument = splited_line[2].split('#')
            argumets_parse(argument[0], 2, 3, instruction)
        else:
            opcode_possible = r'^[A-Z0-9]+$'
            
            if re.match(opcode_possible, opcode_value):
                sys.exit(OPCODE_ERROR)
            else:
                sys.exit(SYN_OR_LEX_ERROR)
            
    return xw
    

if __name__ == "__main__":
    command_line()
    xw = header()
    counter = 0
    xw = main_parse(xw, counter)
    
    output = ET.tostring(xw.getroot(), encoding='unicode')
    dom = xml.dom.minidom.parseString(output)

    #Print the XML
    pretty_xml = dom.toprettyxml(encoding='UTF-8')
    print(pretty_xml.decode('UTF-8'), end='')

    sys.exit(0)

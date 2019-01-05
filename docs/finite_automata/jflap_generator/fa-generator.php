<?php

$keywords = array(
    "if",
    "else",
    "for",
    "while",
    "do",
    "return",
    "char",
    "int",
    "double",
    "bool",
    "void",
    "const",
    "true",
    "false",
    "class",
    "public",
    "private",
);

$num = 0;
$labels = array();

$states = "";
$transitions = "";

foreach ($keywords as $word) {
    for ($i = 0; $i < strlen($word); $i++) {
        $num++;
        $char = substr($word, $i, 1);
        
        if ($i == strlen($word) - 1) {
            $labels[$num] = $word;
        }
        
        $transitions .= "<transition>
                <from>".(($i == 0) ? 0 : ($num - 1))."</from>
                <to>$num</to>
                <read>$char</read>
            </transition>";
    }
}

for ($i = 1; $i <= $num; $i++) {
    $states .= "<state id=\"$i\" name=\"q$i\">
			<x>30.0</x>
			<y>30.0</y>
            ".((isset($labels[$i])) ? "<label>{$labels[$i]}</label>" : "")."
		</state>";
}

echo "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><!--Created with JFLAP 6.4.--><structure>
	<type>fa</type>
	<automaton>
		<!--The list of states.-->
		<state id=\"0\" name=\"q0\">
			<x>30.0</x>
			<y>30.0</y>
			<initial/>
		</state>
		$states
		<!--The list of transitions.-->
		$transitions
	</automaton>
</structure>";
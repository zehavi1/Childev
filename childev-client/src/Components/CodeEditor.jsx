import { forwardRef } from "react";

const CodeEditor = forwardRef(function CodeEditor({ code, onChange }, ref) {
    return (
        <textarea
            ref={ref}
            className="code-editor"
            value={code}
            onChange={(e) => onChange(e.target.value)}
            placeholder={`כתבו כאן קוד...
לדוגמה:
x <- 5
write("hello", x)`}
            spellCheck="false"
        />
    );
});

export default CodeEditor;
// function CodeEditor({ code, onChange }) {
//     return (
//         <textarea
//             className="code-editor"
//             value={code}
//             onChange={(e) => onChange(e.target.value)}
//             placeholder={`כתבו כאן קוד...
// לדוגמה:
// x <- 5
// write("hello", x)`}
//             spellCheck="false"
//         />
//     );
// }

// export default CodeEditor;
const tokenGroups = [
    {
        title: "פקודות",
        icon: "🧩",
        tokens: [
            { label: "write", text: 'write("")', cursorOffset: 7 },
            { label: "read", text: 'read("")', cursorOffset: 6 },
        ]
    },
    {
        title: "תנאים ולולאות",
        icon: "🔁",
        tokens: [
            { label: "if", text: "if  :\n\n||", cursorOffset: 3 },
            { label: "else", text: "else :\n\n||", cursorOffset: 8 },
            { label: "while", text: "while  :\n\n||", cursorOffset: 6 },
            { label: "for", text: "for i <- 1 to 5 :\n\n||", cursorOffset: 19 },
        ]
    },
    {
        title: "סימנים חשובים",
        icon: "✏️",
        tokens: [
            { label: "<-", text: " <- " },
            { label: ":", text: " :\n" },
            { label: "||", text: "\n||\n" },
            { label: "(", text: "(" },
            { label: ")", text: ")" },
            { label: ",", text: ", " },
            { label: `" "`, text: '""', cursorOffset: 1 },
        ]
    },
    {
        title: "השוואות",
        icon: "⚖️",
        tokens: [
            { label: "=", text: " = " },
            { label: "!=", text: " != " },
            { label: "<", text: " < " },
            { label: ">", text: " > " },
            { label: "<=", text: " <= " },
            { label: ">=", text: " >= " },
        ]
    },
    {
        title: "חשבון",
        icon: "➕",
        tokens: [
            { label: "+", text: " + " },
            { label: "-", text: " - " },
            { label: "*", text: " * " },
            { label: "/", text: " / " },
            { label: "^", text: " ^ " },
        ]
    },
    {
        title: "אמת ושקר",
        icon: "✅",
        tokens: [
            { label: "true", text: "true" },
            { label: "false", text: "false" },
            { label: "and &", text: " & " },
            { label: "or |", text: " | " },
        ]
    }
];

function TokenPalette({ onInsert }) {
    return (
        <div className="token-palette">
            <h2>🎮 מקלדת קוד</h2>
            <p>לחצו על מילה או סימן, והוא ייכנס לקוד במקום הסמן.</p>

            {tokenGroups.map((group) => (
                <div className="token-group" key={group.title}>
                    <h3>
                        <span>{group.icon}</span>
                        {group.title}
                    </h3>

                    <div className="token-buttons">
                        {group.tokens.map((token) => (
                            <button
                                key={token.label}
                                className="token-button"
                                onClick={() =>
                                    onInsert(
                                        token.text,
                                        token.cursorOffset ?? token.text.length
                                    )
                                }
                            >
                                {token.label}
                            </button>
                        ))}
                    </div>
                </div>
            ))}
        </div>
    );
}

export default TokenPalette;
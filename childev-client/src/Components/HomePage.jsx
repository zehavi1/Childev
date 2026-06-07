import { useRef, useState } from "react";
import CodeEditor from "./CodeEditor";
import ErrorList from "./CompileErrorList";
import { codeExamples } from "../data/examples";
import { checkCode } from "../services/api";
import RunsHistory from "./RunsHistory";
import TokenPalette from "./TokenPalette";

function HomePage({ currentUser, onLogout }) {
    const [code, setCode] = useState("");
    const [errors, setErrors] = useState([]);
    const [checked, setChecked] = useState(false);
    const [isLoading, setIsLoading] = useState(false);
    const [isSuccess, setIsSuccess] = useState(null);
    const [historyRefreshKey, setHistoryRefreshKey] = useState(0);
    const [isHistoryOpen, setIsHistoryOpen] = useState(false);

    const editorRef = useRef(null);

    function handleInsertText(text, cursorOffset) {
        const textarea = editorRef.current;

        if (!textarea) {
            setCode((prevCode) => prevCode + text);
            return;
        }

        const start = textarea.selectionStart;
        const end = textarea.selectionEnd;

        const before = code.substring(0, start);
        const after = code.substring(end);

        const newCode = before + text + after;
        const newCursorPosition = start + cursorOffset;

        setCode(newCode);

        setTimeout(() => {
            textarea.focus();
            textarea.setSelectionRange(newCursorPosition, newCursorPosition);
        }, 0);
    }

    async function handleCheckCode() {
        if (!code.trim()) {
            setErrors([
                {
                    line: 1,
                    column: 1,
                    type: "empty",
                    message: "עוד לא כתבת קוד. אפשר להתחיל מדוגמה מוכנה."
                }
            ]);

            setChecked(true);
            setIsSuccess(false);
            return;
        }

        if (!currentUser || !currentUser.id) {
            alert("יש להתחבר לפני בדיקת קוד");
            return;
        }

        setIsLoading(true);
        setErrors([]);
        setChecked(false);
        setIsSuccess(null);

        try {
            const result = await checkCode(currentUser.id, code);

            setErrors(result.errors || []);
            setIsSuccess(result.isSuccess);
            setHistoryRefreshKey((prev) => prev + 1);
            setChecked(true);
        } catch (error) {
            console.error(error);

            setErrors([
                {
                    line: 0,
                    column: 0,
                    type: "Server",
                    message: "שגיאה בשליחת הקוד לשרת"
                }
            ]);

            setIsSuccess(false);
            setChecked(true);
        } finally {
            setIsLoading(false);
        }
    }

    function handleLoadExample(exampleCode) {
        setCode(exampleCode);
        setErrors([]);
        setChecked(false);
        setIsSuccess(null);

        setTimeout(() => {
            editorRef.current?.focus();
        }, 0);
    }

    function handleClear() {
        setCode("");
        setErrors([]);
        setChecked(false);
        setIsSuccess(null);

        setTimeout(() => {
            editorRef.current?.focus();
        }, 0);
    }

    return (
        <div className="app-page">
            <header className="top-header">
                <div className="welcome-area">
                    <div className="welcome-icon">🌈</div>

                    <div>
                        <h1>שלום ל־{currentUser.displayName} 🌟</h1>
                        <p>אפשר לכתוב קוד לבד, או להיעזר במקלדת הקוד.</p>
                    </div>
                </div>

                <div className="header-actions">
                    <button
                        className="history-button"
                        onClick={() => setIsHistoryOpen(true)}
                    >
                        🕘 היסטוריה
                    </button>

                    <button className="logout-button" onClick={onLogout}>
                        יציאה
                    </button>
                </div>
            </header>

            <main className="workspace-layout">
                <aside className="keyboard-column">
                    <section className="side-card keyboard-card">
                        <TokenPalette onInsert={handleInsertText} />
                    </section>
                </aside>

                <section className="editor-column">
                    <section className="editor-card">
                        <div className="card-title-row">
                            <div>
                                <h2>✍️ אזור כתיבת הקוד</h2>
                                <p className="card-subtitle">
                                    כתבו כאן את הקוד שלכם בשפת Childev
                                </p>
                            </div>

                            <span className="cute-badge">Childev</span>
                        </div>

                        <CodeEditor
                            ref={editorRef}
                            code={code}
                            onChange={setCode}
                        />

                        <div className="actions-row">
                            <button
                                className="check-button"
                                onClick={handleCheckCode}
                                disabled={isLoading}
                            >
                                {isLoading ? "⏳ בודק..." : "🚀 בדיקת קוד"}
                            </button>

                            <button
                                className="clear-button"
                                onClick={handleClear}
                                disabled={isLoading}
                            >
                                🧹 ניקוי
                            </button>
                        </div>
                    </section>

                    <section className="results-card">
                        <div className="card-title-row">
                            <div>
                                <h2>🧩 מה קרה בקוד?</h2>
                                <p className="card-subtitle">
                                    כאן תופיע תוצאת הבדיקה
                                </p>
                            </div>
                        </div>

                        {/* {checked && isSuccess === true && (
                            <div className="success-message">
                                ✅ הקוד תקין! לא נמצאו שגיאות.
                            </div>
                        )} */}

                        <ErrorList
                            checked={checked}
                            errors={errors}
                        />
                    </section>
                </section>

                <aside className="examples-column">
                    <section className="side-card examples-card">
                        <h2>📚 דוגמאות מוכנות</h2>
                        <p>בחרו דוגמה, שנו אותה, ואז בדקו את הקוד.</p>

                        <div className="examples-list">
                            {codeExamples.map((example) => (
                                <button
                                    key={example.id}
                                    className="example-button"
                                    onClick={() => handleLoadExample(example.code)}
                                >
                                    <span className="example-icon">
                                        {example.icon}
                                    </span>

                                    <div>
                                        <strong>{example.title}</strong>
                                        <small>{example.description}</small>
                                    </div>
                                </button>
                            ))}
                        </div>
                    </section>
                </aside>
            </main>

            {isHistoryOpen && (
                <div
                    className="history-modal-overlay"
                    onClick={() => setIsHistoryOpen(false)}
                >
                    <div
                        className="history-modal"
                        onClick={(event) => event.stopPropagation()}
                    >
                        <div className="history-modal-header">
                            <div>
                                <h2>🕘 היסטוריית בדיקות</h2>
                                <p>כאן אפשר לראות בדיקות קוד קודמות.</p>
                            </div>

                            <button
                                className="history-close-button"
                                onClick={() => setIsHistoryOpen(false)}
                            >
                                ✕
                            </button>
                        </div>

                        <RunsHistory
                            currentUser={currentUser}
                            refreshKey={historyRefreshKey}
                        />
                    </div>
                </div>
            )}
        </div>
    );
}

export default HomePage;
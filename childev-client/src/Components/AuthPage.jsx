import { useState } from "react";
import { login, register } from "../services/api";

function AuthPage({ onLogin }) {
    const [mode, setMode] = useState("login");

    const [displayName, setDisplayName] = useState("");
    const [username, setUsername] = useState("");
    const [password, setPassword] = useState("");

    const [message, setMessage] = useState("");
    const [isLoading, setIsLoading] = useState(false);

    const isLogin = mode === "login";

    function clearFields() {
        setDisplayName("");
        setUsername("");
        setPassword("");
        setMessage("");
    }

    function switchMode(newMode) {
        setMode(newMode);
        clearFields();
    }

    async function handleSubmit(event) {
        event.preventDefault();

        if (!username.trim() || !password.trim()) {
            setMessage("יש למלא שם משתמש וסיסמה");
            return;
        }

        if (!isLogin && !displayName.trim()) {
            setMessage("יש למלא שם לתצוגה");
            return;
        }

        setIsLoading(true);
        setMessage("");

        try {
            let result;

            if (isLogin) {
                result = await login(username, password);
            } else {
                result = await register(username, password, displayName);
            }

            if (result.success) {
                onLogin(result.user);
            } else {
                setMessage(result.message || "הפעולה נכשלה");
            }
        } catch (error) {
            console.error(error);
            setMessage("שגיאה בתקשורת עם השרת");
        } finally {
            setIsLoading(false);
        }
    }

    return (
        <div className="auth-page">
            <div className="floating-shape shape-one">⭐</div>
            <div className="floating-shape shape-two">🚀</div>
            <div className="floating-shape shape-three">🎈</div>

            <div className="auth-card">
                <div className="logo-circle">🌈</div>

                <h1>ברוכים הבאים ל־Childev</h1>
                <p className="auth-subtitle">
                    המקום שבו ילדים לומדים לכתוב קוד בכיף
                </p>

                <div className="tabs">
                    <button
                        type="button"
                        className={isLogin ? "tab active" : "tab"}
                        onClick={() => switchMode("login")}
                        disabled={isLoading}
                    >
                        כניסה
                    </button>

                    <button
                        type="button"
                        className={!isLogin ? "tab active" : "tab"}
                        onClick={() => switchMode("register")}
                        disabled={isLoading}
                    >
                        הרשמה
                    </button>
                </div>

                <form onSubmit={handleSubmit} className="auth-form">
                    {!isLogin && (
                        <div className="form-group">
                            <label>איך לקרוא לך?</label>
                            <input
                                type="text"
                                value={displayName}
                                onChange={(e) => setDisplayName(e.target.value)}
                                placeholder="למשל: תמר"
                                disabled={isLoading}
                            />
                        </div>
                    )}

                    <div className="form-group">
                        <label>שם משתמש</label>
                        <input
                            type="text"
                            value={username}
                            onChange={(e) => setUsername(e.target.value)}
                            placeholder="שם משתמש"
                            disabled={isLoading}
                        />
                    </div>

                    <div className="form-group">
                        <label>סיסמה</label>
                        <input
                            type="password"
                            value={password}
                            onChange={(e) => setPassword(e.target.value)}
                            placeholder="סיסמה"
                            disabled={isLoading}
                        />
                    </div>

                    {message && (
                        <div className="auth-message">
                            {message}
                        </div>
                    )}

                    <button
                        className="main-auth-button"
                        type="submit"
                        disabled={isLoading}
                    >
                        {isLoading
                            ? "מבצע פעולה..."
                            : isLogin
                                ? "🚀 כניסה למערכת"
                                : "✨ הרשמה והתחלה"}
                    </button>
                </form>

                <div className="auth-hint">
                    {isLogin ? (
                        <>
                            אין לך משתמש עדיין?{" "}
                            <button
                                type="button"
                                onClick={() => switchMode("register")}
                                disabled={isLoading}
                            >
                                להרשמה
                            </button>
                        </>
                    ) : (
                        <>
                            כבר יש לך משתמש?{" "}
                            <button
                                type="button"
                                onClick={() => switchMode("login")}
                                disabled={isLoading}
                            >
                                לכניסה
                            </button>
                        </>
                    )}
                </div>
            </div>
        </div>
    );
}

export default AuthPage;



// import { useState } from "react";

// function AuthPage({ onLogin, onRegister }) {
//     const [mode, setMode] = useState("login");

//     const [displayName, setDisplayName] = useState("");
//     const [username, setUsername] = useState("");
//     const [password, setPassword] = useState("");

//     const [message, setMessage] = useState("");

//     const isLogin = mode === "login";

//     function clearFields() {
//         setDisplayName("");
//         setUsername("");
//         setPassword("");
//         setMessage("");
//     }

//     function switchMode(newMode) {
//         setMode(newMode);
//         clearFields();
//     }

//     function handleSubmit(event) {
//         event.preventDefault();

//         let result;

//         if (isLogin) {
//             result = onLogin(username, password);
//         } else {
//             result = onRegister(displayName, username, password);
//         }

//         if (!result.success) {
//             setMessage(result.message);
//         }
//     }

//     return (
//         <div className="auth-page">
//             <div className="floating-shape shape-one">⭐</div>
//             <div className="floating-shape shape-two">🚀</div>
//             <div className="floating-shape shape-three">🎈</div>

//             <div className="auth-card">
//                 <div className="logo-circle">🌈</div>

//                 <h1>ברוכים הבאים ל־Childev</h1>
//                 <p className="auth-subtitle">
//                     המקום שבו ילדים לומדים לכתוב קוד בכיף
//                 </p>

//                 <div className="tabs">
//                     <button
//                         className={isLogin ? "tab active" : "tab"}
//                         onClick={() => switchMode("login")}
//                     >
//                         כניסה
//                     </button>

//                     <button
//                         className={!isLogin ? "tab active" : "tab"}
//                         onClick={() => switchMode("register")}
//                     >
//                         הרשמה
//                     </button>
//                 </div>

//                 <form onSubmit={handleSubmit} className="auth-form">
//                     {!isLogin && (
//                         <div className="form-group">
//                             <label>איך לקרוא לך?</label>
//                             <input
//                                 type="text"
//                                 value={displayName}
//                                 onChange={(e) => setDisplayName(e.target.value)}
//                                 placeholder="למשל: תמר"
//                             />
//                         </div>
//                     )}

//                     <div className="form-group">
//                         <label>שם משתמש</label>
//                         <input
//                             type="text"
//                             value={username}
//                             onChange={(e) => setUsername(e.target.value)}
//                             placeholder="שם משתמש"
//                         />
//                     </div>

//                     <div className="form-group">
//                         <label>סיסמה</label>
//                         <input
//                             type="password"
//                             value={password}
//                             onChange={(e) => setPassword(e.target.value)}
//                             placeholder="סיסמה"
//                         />
//                     </div>

//                     {message && (
//                         <div className="auth-message">
//                             {message}
//                         </div>
//                     )}

//                     <button className="main-auth-button" type="submit">
//                         {isLogin ? "🚀 כניסה למערכת" : "✨ הרשמה והתחלה"}
//                     </button>
//                 </form>

//                 <div className="auth-hint">
//                     {isLogin ? (
//                         <>
//                             אין לך משתמש עדיין?{" "}
//                             <button onClick={() => switchMode("register")}>
//                                 להרשמה
//                             </button>
//                         </>
//                     ) : (
//                         <>
//                             כבר יש לך משתמש?{" "}
//                             <button onClick={() => switchMode("login")}>
//                                 לכניסה
//                             </button>
//                         </>
//                     )}
//                 </div>

//                 <div className="demo-users">
//                     <strong>משתמש לדוגמה:</strong>
//                     <span>teacher / 1234</span>
//                 </div>
//             </div>
//         </div>
//     );
// }

// export default AuthPage;
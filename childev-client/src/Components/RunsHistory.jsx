import { useEffect, useState } from "react";
import { getRuns } from "../services/api";
import "../App.css";

function RunsHistory({ currentUser, refreshKey }) {
    const [isOpen, setIsOpen] = useState(false);
    const [runs, setRuns] = useState([]);
    const [isLoading, setIsLoading] = useState(false);
    const [message, setMessage] = useState("");

    useEffect(() => {
        loadRuns();
    }, [currentUser?.id, refreshKey]);

    async function loadRuns() {
        if (!currentUser?.id) {
            return;
        }

        setIsLoading(true);
        setMessage("");

        try {
            const data = await getRuns(currentUser.id);
            setRuns(data || []);
        } catch (error) {
            console.error(error);
            setMessage("לא הצלחנו לטעון את היסטוריית ההרצות");
        } finally {
            setIsLoading(false);
        }
    }

    return (
        <>
            <button
                className="history-toggle-button"
                onClick={() => setIsOpen(true)}
            >
                🕘 היסטוריה
            </button>

            {isOpen && (
                <div className="history-overlay" onClick={() => setIsOpen(false)}>
                    <aside
                        className="history-panel"
                        onClick={(e) => e.stopPropagation()}
                    >
                        <div className="history-header">
                            <div>
                                <h2>🕘 היסטוריית הרצות</h2>
                                <p>כל בדיקות הקוד שביצעת במערכת</p>
                            </div>

                            <button
                                className="history-close-button"
                                onClick={() => setIsOpen(false)}
                            >
                                ✖
                            </button>
                        </div>

                        <button
                            className="history-refresh-button"
                            onClick={loadRuns}
                            disabled={isLoading}
                        >
                            {isLoading ? "טוען..." : "רענון"}
                        </button>

                        {message && (
                            <div className="history-message">
                                {message}
                            </div>
                        )}

                        {runs.length === 0 && !isLoading ? (
                            <p className="history-empty">
                                עדיין אין הרצות קוד.
                            </p>
                        ) : (
                            <div className="history-list">
                                {runs.map((run) => (
                                    <div
                                        key={run.id}
                                        className={
                                            run.isSuccess
                                                ? "history-run-card success"
                                                : "history-run-card failed"
                                        }
                                    >
                                        <div className="history-run-top">
                                            <strong>
                                                {run.isSuccess ? "✅ הצליח" : "❌ נכשל"}
                                            </strong>

                                            <span>
                                                #{run.id}
                                            </span>
                                        </div>

                                        <p>
                                            תאריך:{" "}
                                            {new Date(run.createdAt).toLocaleString("he-IL")}
                                        </p>

                                        <p>
                                            מספר שגיאות: {run.errorsCount}
                                        </p>
                                    </div>
                                ))}
                            </div>
                        )}
                    </aside>
                </div>
            )}
        </>
    );
}

export default RunsHistory;
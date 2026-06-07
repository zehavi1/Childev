import { useState } from "react";
import AuthPage from "./components/AuthPage";
import HomePage from "./components/HomePage";
import "./App.css";
import "./styles/app.css";

function App() {
    const [currentUser, setCurrentUser] = useState(() => {
        const savedUser = localStorage.getItem("currentUser");
        return savedUser ? JSON.parse(savedUser) : null;
    });

    function handleLogin(user) {
        setCurrentUser(user);
        localStorage.setItem("currentUser", JSON.stringify(user));
    }

    function handleLogout() {
        setCurrentUser(null);
        localStorage.removeItem("currentUser");
    }

    if (!currentUser) {
        return (
            <AuthPage
                onLogin={handleLogin}
            />
        );
    }

    return (
        <HomePage
            currentUser={currentUser}
            onLogout={handleLogout}
        />
    );
}

export default App;
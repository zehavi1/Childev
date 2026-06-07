const API_BASE_URL = "https://localhost:50873/api";
// אם השרת רץ אצלך בפורט אחר, לשנות כאן.
// לדוגמה: https://localhost:7182/api

async function handleResponse(response) {
  const data = await response.json();

  if (!response.ok) {
    throw data;
  }

  return data;
}

export async function register(userName, password, displayName) {
  const response = await fetch(`${API_BASE_URL}/Auth/register`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({
      userName,
      password,
      displayName
    })
  });

  return handleResponse(response);
}

export async function login(userName, password) {
  const response = await fetch(`${API_BASE_URL}/Auth/login`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({
      userName,
      password
    })
  });

  return handleResponse(response);
}

export async function checkCode(userId, code) {
  const response = await fetch(`${API_BASE_URL}/Childev/check`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify({
      userId,
      code
    })
  });

  return handleResponse(response);
}

export async function getExamples() {
  const response = await fetch(`${API_BASE_URL}/examples`);
  return handleResponse(response);
}

export async function getRuns(userId) {
  const response = await fetch(`${API_BASE_URL}/Childev/runs/${userId}`);
  return handleResponse(response);
}
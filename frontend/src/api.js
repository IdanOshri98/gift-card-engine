const API_URL = import.meta.env.VITE_API_URL || "http://localhost:8080";
const BASE_URL = `${API_URL}/api`;

async function throwIfError(res) {
  if (!res.ok) {
    let message = "Request failed";
    try {
      const body = await res.json();
      if (body && body.error) {
        message = body.error;
      }
    } catch {
      // ignore invalid JSON body, fall back to generic message
    }
    throw new Error(message);
  }
}

export async function getCards() {
  const res = await fetch(`${BASE_URL}/cards`);
  return res.json();
}

export async function addCard(card) {
  const res = await fetch(`${BASE_URL}/cards`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(card),
  });
  await throwIfError(res);
  return res.json();
}

export async function updateCard(id, data) {
  const res = await fetch(`${BASE_URL}/cards/${id}`, {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(data),
  });
  await throwIfError(res);
  return res.json();
}

export async function deleteCard(id) {
  const res = await fetch(`${BASE_URL}/cards/${id}`, { method: "DELETE" });
  await throwIfError(res);
  return res.json();
}

export async function getRiskCards() {
  const res = await fetch(`${BASE_URL}/cards/risk`);
  return res.json();
}

export async function getPlan(by = "expiry") {
  const res = await fetch(`${BASE_URL}/cards/plan?by=${by}`);
  return res.json();
}

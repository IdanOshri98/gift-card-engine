import { addCard } from "../api";
import CardForm from "./CardForm";

const EMPTY_FORM = { title: "", balance: "", expiryDate: "", companies: "" };

export default function AddCard({ onAdd }) {
  async function handleSubmit(card) {
    const result = await addCard(card);
    if (!result.error) onAdd();
    return result;
  }

  return (
    <CardForm
      heading="Add Card"
      initialValues={EMPTY_FORM}
      requireFields
      submitLabel="Add Card"
      onSubmit={handleSubmit}
    />
  );
}

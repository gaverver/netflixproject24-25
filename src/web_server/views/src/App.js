import './App.css';
import { Routes, Route } from 'react-router-dom';
import Menu from './components/Menu/Menu';
import SearchResults from './pages/SearchResults/SearchResults';

function App() {
  return (
    <div className="App">
      <Menu
        username={"gavri"}
        photo={'/images_for_decoration/search_icon.jpg'}
        isAdmin={true}
      />
      <Routes>
        <Route
          path="/movies/search/:query"
          element={<SearchResults />}
        />
      </Routes>
    </div>
  );
}

export default App;
